//#pragma GCC target ("arm")
#pragma GCC target("thumb")

void csd_main()
{
	int indata[32] = { 2, 0, -7, -1, 3, 8, -4, 10,
						-9, -16, 15, 13, 1, 4, -3, 14,
						-8, -10, -15, 6, -13, -5, 9, 12,
						-11, -14, -6, 11, 5, 7, -2, -12 };
	int outdata[32];

	// ������ �̵�
	for (int i = 0; i < 32; i++) {
		outdata[i] = indata[i];
	}

	// ���� ���� �̿�
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32 - i - 1; j++) {
			// �տ� �ִ� �������� ���� �� ũ�ٸ� ��ġ ����
			if (outdata[j] > outdata[j + 1]) {
				int tmp = outdata[j];
				outdata[j] = outdata[j + 1];
				outdata[j + 1] = tmp;
			}
		}
	}

}
