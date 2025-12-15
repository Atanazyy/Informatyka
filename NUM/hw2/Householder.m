function [x,R,B] = Householder(A,y)
    [m,n] = size(A);
    B = zeros(m,n);

    for k = 1:n
        % Obliczenie wektora Householdera
        xk = A(k:m,k);
        v = xk;
        sig = sign(xk(1));
        if (sig == 0)
            sig = 1;
        endif
        v(1) = v(1) + sig * norm(xk);
        lambda = 2 / (v' * v);
        % Aktualizacja macierzy A
        A(k:m,k:n) = A(k:m,k:n) - lambda * v * (v' * A(k:m,k:n));
        % Aktualizacja wektora y
        y(k:m) = y(k:m) - lambda * v * (v' * y(k:m));
        % Zapis wektora Householdera do B
        B(k:m,k) = v;
    endfor

    R = A(1:n,1:n);
    x = R \ y(1:n);
end

