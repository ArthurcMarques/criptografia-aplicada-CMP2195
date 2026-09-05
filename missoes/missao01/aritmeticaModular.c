int aritmetica_modular(int num, int x)
{
    int m;

    if (x <= 0)
    {
        return -1;
    }

    m = num % x;

    if (m < 0)
    {
        m = m + x;
    }

    return m;
}