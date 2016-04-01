

int main()
{
    // set priority to lowest

    loop:
        asm volatile("sti");
        asm volatile("hlt");
    goto loop;
}
