L = 18;
m = zeros(L,L);
n = 3;

for i = 1:L
    m(i,i) = 4;
end

for i = 2:L
    m(i-1,i) = 1;
    m(i,i-1) = 1;
end

for i = n+1:L
    m(i-n,i) = 1;
    m(i,i-n) = 1;
end




m

ans = inv(m)

res = ans*m