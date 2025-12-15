function [x, k] = gs_iter(A, b, x0, tol=1e-6, maxiter=1e4)
    x = x0;
    k = 0;
    L = tril(A);
    U = A - L;
    normb = norm(b);

    while k < maxiter
        x = L \ (b - U * x);
        k = k + 1;
        if norm(b - A * x) <= tol * normb
            return;
        end
    end
end
