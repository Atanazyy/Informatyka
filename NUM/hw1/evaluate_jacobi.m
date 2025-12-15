function evaluate_jacobi(A, b, x0, x_star)
    tic;
    [x, k] = jacobi_iter(A, b, x0);
    time = toc;
    rel_err = norm(x - x_star) / norm(x_star);
    res_err = norm(b - A * x);
    printf("Jacobi: liczba iteracji = %d,  błąd względny = %.3e,  błąd rezydualny = %.3e,  czas = %.3f s\n", k, rel_err, res_err, time);
end
