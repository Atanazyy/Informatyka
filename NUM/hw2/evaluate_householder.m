function evaluate_householder(m, eps=1e-2)
    x = linspace(0, 10, m);
    e = 2 * eps * rand(1, m) - eps;
    y = polyval([1 -5 2], x) + e;
    X = [x'.^2, x'.^1, x'.^0];
    [p_est, R, B] = Householder(X, y');
    abs_error = abs([1; -5; 2] - p_est);
    fprintf('Estymowane współczynniki wielomianu: a=%.6f, b=%.6f, c=%.6f\n', p_est(1), p_est(2), p_est(3));
    fprintf('Błędy bezwzględne: %.6f, %.6f, %.6f\n', abs_error(1), abs_error(2), abs_error(3));
    fprintf('Błąd rozkładu QR: %.3e\n', calculate_error(X, R, B));
end
