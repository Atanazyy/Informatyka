int graj(int a, int b)
{
    if((a + b) % 2 == 0)
    {
        if(a != 1)
            return 1;
        else 
            return 2;
    }
    if(a % 2 == 0)
        return 1;
    else 
        return 2;
}