function evaluate_gs(A, b, x0, x_star)
    tic;
    [x, k] = gs_iter(A, b, x0);
    time = toc;
    rel_err = norm(x - x_star) / norm(x_star);
    res_err = norm(b - A * x);
    printf("Gauss-Seidel: liczba iteracji = %d,  błąd względny = %.3e,  błąd rezydualny = %.3e,  czas = %.3f s\n", k, rel_err, res_err, time);
end
