const int n = 1e8;
bool tab[n + 1];

int main()
{
  for (int i = 2; i <= n; i++) {
    tab[i] = true;
  }
  long long sum = 0;
  for (int i = 2; i <= n; i++) {
    if (tab[i]) {
      sum += i;
      for (int j = i + i; j <= n; j += i) {
        tab[j] = false;
      }
    }
  }
  //printf("%lld", sum);
}
