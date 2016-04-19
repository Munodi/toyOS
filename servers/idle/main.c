

int main()
{
    // set priority to lowest

    loop:
        asm volatile("hlt");
    goto loop;
}
