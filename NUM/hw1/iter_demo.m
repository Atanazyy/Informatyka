printf("\n======================== (a): Macierz C + pI ========================\n");
ns = [100, 1000];
ps = [5, 10, 100];
for n = ns
    C = rand(n);
    for p = ps
        printf("n = %d, p = %d:\n", n, p);
        x0 = zeros(n, 1);
        x_star = ones(n, 1);
        A = C + p * eye(n);
        b = A * x_star;
        evaluate_jacobi(A, b, x0, x_star);
        evaluate_gs(A, b, x0, x_star);
    end
end

printf("\n======================== (b): Macierz Hilberta ========================\n");
ns = [5, 10, 20];
for n = ns
    printf("n = %d:\n", n);
    x0 = zeros(n, 1);
    x_star = ones(n, 1);
    A = hilb(n);
    b = A * x_star;
    evaluate_jacobi(A, b, x0, x_star);
    evaluate_gs(A, b, x0, x_star);
end

printf("\n==================== (c): Macierz trójdiagonalna ====================\n");
ns = [100, 1000];
ps = [2, 3];
for n = ns
    for p = ps
        printf("n = %d, p = %d:\n", n, p);
        x0 = zeros(n, 1);
        x_star = ones(n, 1);
        diagonals = [-1 * ones(n,1), p * ones(n,1), -1 * ones(n,1)];
        A = spdiags(diagonals, [-1, 0, 1], n, n);
        b = A * x_star;
        evaluate_jacobi(A, b, x0, x_star);
        evaluate_gs(A, b, x0, x_star);
    end
end

