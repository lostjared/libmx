



int swizzle(void *buffer, unsigned int size)
{
	unsigned int i = 0;
	while(++i < size)
	{
		unsigned char *ptr = (unsigned char*) buffer+i;
		unsigned char c = *ptr;
		c = ( c << 1 | c >> 31 );
		c ^= 3;
		*ptr = c;		
	}
	return 0;
}