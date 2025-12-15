function err = calculate_error(A, R, B)
    [m,n] = size(A);
    QR = [R; zeros(m-n, n)];
    for k = n:-1:1
        v = B(k:m, k);
        lambda = 2 / (v' * v);
        QR(k:m, :) = QR(k:m, :) - lambda * v * (v' * QR(k:m, :));
    endfor
    err = norm(A - QR) / norm(A);
end
