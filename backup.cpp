#if TESTCASE == 001

	while(1)
	{
		PORTC = 0x10;
		PORTD = ~status[d3];
		PORTB = ~status[d3];
		_delay_us(10);
		PORTC = 0x08;
		PORTD = ~status[d2];
		PORTB = ~status[d2];
		_delay_us(10);
		PORTC = 0x04;
		PORTD = ~status[d1];
		PORTB = ~status[d1];
		_delay_us(10);

	}
#endif

	
#if TESTCASE == 002		// Testing case
	
	while (1)
	{
		for (int c=0; c <= 4; )
		{
			// h1
			if (c == 1)
			{
				PORTC = 0x02;
				PORTB = ~status[d1];
				PORTD = ~status[d1];
			}		
			// h2
			else if (c == 2)
			{
				PORTC = 0x04;
				PORTB = ~status[d2];
				PORTD = ~status[d2];
			}		
			// m1
			else if (c == 3)
			{
				PORTC = 0x08;
				PORTB = ~status[d3];
				PORTD = ~status[d3];
			}		
			// m2
			else if (c == 4)
			{
				PORTC = 0x10;
				PORTB = ~status[d4];
				PORTD = ~status[d4];
				c = 0;
			}
			_delay_us(100);
			c++;
		}
	}
#endif		
		
		
#if TESTCASE == 003		// 4 digits displaying 24 hours
	while(1)
	{
//						 0	   1     2     3     4     5     6     7     8     9    DOT
		int status[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80, 0x00, 0x30};
		int m2 = 0;
		int m1 = 0;
		int h2 = 0;
		int h1 = 0;
		int checktime = 0;
		
//		PORTC = 0x10;
		
		for (h1=0; h1 <= 2; )
		{
			for (h2=0; h2 <= 10; )
			{
				for (m1=0; m1 <= 6; )
				{
					for (m2=0; m2 <= 10; )
					{
						PORTC = 0x10;
						PORTB = ~(status[m2]);
						PORTD = ~(status[m2]);
						_delay_us(10);
						PORTC = 0x08;
						PORTB = ~(status[m1]);
						PORTD = ~(status[m1]);
						_delay_us(10);
						PORTC = 0x04;
						PORTB = ~(status[h2]);
						PORTD = ~(status[h2]);
						_delay_us(10);
						PORTC = 0x02;
						PORTB = ~(status[h1]);
						PORTD = ~(status[h1]);
						_delay_us(10);
					
						if (TCNT1 >= 31249)							
						{
							TCNT1 = 0; // Reset Timer 1 value
							NoCount ++;
							if (NoCount == 1)
							{
								NoCount = 0;
								m2++;
								if (m2 == 10)
								{
									m2 = 0;
									m1++;
									if (m1 == 6)
									{
										m1 = 0;
										h2++;
										if (h2 == 10)
										{
											h2 = 0;
											h1++;
											if (h1 == 3)
											{
												h1 = 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
#endif 
