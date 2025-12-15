function [x, k] = jacobi_iter(A, b, x0, tol=1e-6, maxiter=1e4)
    x = x0;
    k = 0;
    D = diag(A);
    R = A - diag(D);
    normb = norm(b);

    while k < maxiter
        x = (b - R * x) ./ D;
        k = k + 1;
        if norm(b - A * x) <= tol * normb
            return;
        end
    end
end
