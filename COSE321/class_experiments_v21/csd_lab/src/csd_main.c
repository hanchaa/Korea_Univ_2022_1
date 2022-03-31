void csd_main()
{
	unsigned char * switches = (unsigned char *)0x41210000;
	int loop = 100000000;

	if (*switches & 0b10000000)
		loop = 10000000;

	else if (*switches & 0b1000000)
		loop = 20000000;

	else if (*switches & 0b100000)
		loop = 30000000;

	else if (*switches & 0b10000)
		loop = 40000000;

	else if (*switches & 0b1000)
		loop = 50000000;

	else if (*switches & 0b100)
		loop = 60000000;

	else if (*switches & 0b10)
		loop = 70000000;

	else if (*switches & 0b1)
		loop = 80000000;

	for (int i = 0; i < loop; i++){
	}
}
