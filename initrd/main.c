void main(int argc, char** argv)
{
	//int a = 5 / 0;
	__asm("int $80");
	for(;;);
}
