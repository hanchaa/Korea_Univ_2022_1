void csd_main()
{
	unsigned char * switches = (unsigned char *)0x41210000; // 스위치의 메모리 주소 load
	int loop = 100000000; // 1억번 loop를 돌면 대략 1초

	// 몇 번째 스위치가 켜져있는지 확인 후 그에 맞게 loop 횟수 결정
	// 높은 번호의 스위치부터 확인하여 우선순위 부여
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

	// 일정시간 동안 대기를 위한 loop
	for (int i = 0; i < loop; i++){
	}
}
