const tab = [];
var n = 1e8;
for (let i = 2; i <= n; i++) {
    tab[i] = true;
}
var sum = 0;
for (let i = 2; i <= n; i++) {
    if (tab[i]) {
        for (let j = i + i; j <= n; j += i) {
            tab[j] = false;
        }
        sum += i;
    }
}
console.log(sum);

