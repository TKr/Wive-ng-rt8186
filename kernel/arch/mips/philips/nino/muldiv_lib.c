/*
**	mul and div emulation
*/


#ifdef _U32
#undef _U32
#endif

typedef unsigned long _U32;

#define GET4BIT(num,offset)		((num & (0xf << (offset * 4))) >> (offset * 4))

__inline void add_64_64_unsigned(_U32 s1hi, _U32 s1lo, _U32 s2hi, _U32 s2lo, _U32 *dhi, _U32 *dlo)
{
	int		i;
	_U32	r, a = 0, lo = 0;
	
	for(i=0;i<8;i++)
	{
		r	= GET4BIT(s1lo,i) + GET4BIT(s2lo,i) + a;
		a	= r>>4;
		
		lo	|= (r&0xf) << (i*4);
	}
	*dlo	= lo;
	*dhi	= s1hi+s2hi+a;
}


_U32 shift_mask[] = 
{ 0, 
  0x1,				0x3,			0x7,			0xf, 
  0x1f,				0x3f,			0x7f,			0xff,
  0x1ff,			0x3ff,			0x7ff,			0xfff,
  0x1fff, 			0x3fff,			0x7fff,			0xffff,
  0x1ffff,			0x3ffff,		0x7ffff,		0xfffff,
  0x1fffff,			0x3fffff,		0x7fffff,		0xffffff, 
  0x1ffffff,		0x3ffffff,		0x7ffffff,		0xfffffff,
  0x1fffffff,		0x3fffffff,		0x7fffffff,		0xffffffff,};

							

__inline void shift_left_64(_U32 s1hi, _U32 s1lo, int bits, _U32 *dhi, _U32 *dlo)
{
	_U32	tmp, mask = 0;
	int		i;
	
	mask	= shift_mask[bits];
	
	if(mask)
		mask <<= 32-bits;
		
	tmp	= (s1lo & mask) >> (32-bits);
	
	*dlo	= s1lo << bits;
	*dhi	= (s1hi << bits) | tmp;
}

__inline void mul_32_4_unsigned(_U32 s1, unsigned char s2, _U32 *dhi, _U32 *dlo)
{
	int		i;
	_U32	thi, tlo, rhi, rlo;
	
	rhi	= rlo = 0;
	
	if(s2 & 1)
	{
		rlo	= s1;
		rhi	= 0;
	}
	
	if(s2 & 2)
	{
		shift_left_64(0, s1, 1, &thi, &tlo);
		add_64_64_unsigned(rhi, rlo, thi, tlo, &rhi, &rlo);
	}
	
	if(s2 & 4)
	{
		shift_left_64(0, s1, 2, &thi, &tlo);
		add_64_64_unsigned(rhi, rlo, thi, tlo, &rhi, &rlo);
	}
	
	if(s2 & 8)
	{
		shift_left_64(0, s1, 3, &thi, &tlo);
		add_64_64_unsigned(rhi, rlo, thi, tlo, &rhi, &rlo);
	}

	if(s2 & 0x10)
	{
		shift_left_64(0, s1, 4, &thi, &tlo);
		add_64_64_unsigned(rhi, rlo, thi, tlo, &rhi, &rlo);
	}
	
	*dlo	= rlo;
	*dhi	= rhi;
}

__inline _U32 mul_32_32_unsigned(_U32 s1, _U32 s2, _U32 *dhi, _U32 *dlo)
{
	int		i;
	_U32	hi[8], lo[8], thi, tlo;
	
	mul_32_4_unsigned(s1, (unsigned char)GET4BIT(s2,0), &hi[0], &lo[0]);
	for(i=1;i<8;i++)
	{
		mul_32_4_unsigned(s1, (unsigned char)GET4BIT(s2,i), &thi, &tlo);
		shift_left_64(thi, tlo, i*4, &hi[i], &lo[i]);
	}
	
	thi	= hi[0];
	tlo	= lo[0];
	for(i=1;i<8;i++)
	{
		add_64_64_unsigned(hi[i], lo[i], thi, tlo, &thi, &tlo);
	}
	
	*dhi	= thi;
	*dlo	= tlo;
	
	return	tlo;
}

__inline _U32 mul_32_32_signed(_U32 s1, _U32 s2, _U32 *dhi, _U32 *dlo)
{
	if((s1&0x80000000) && (s2&0x80000000))
	{
		mul_32_32_unsigned(0-s1, 0-s2, dhi, dlo);
		
		return *dlo;
	}
	else if(!(s1&0x80000000) && !(s2&0x80000000))
	{
		mul_32_32_unsigned(s1, s2, dhi, dlo);
		
		return *dlo;
	}
	else if(!(s1&0x80000000) && (s2&0x80000000))
	{
		mul_32_32_unsigned(s1, 0-s2, dhi, dlo);
		
		*dhi	= ~*dhi;
		*dlo	= ~*dlo;
		
		add_64_64_unsigned(*dhi, *dlo, 0, 1, dhi, dlo);
		
		return *dlo;
	}
	else if((s1&0x80000000) && !(s2&0x80000000))
	{
		mul_32_32_unsigned(0-s1, s2, dhi, dlo);
		
		*dhi	= ~*dhi;
		*dlo	= ~*dlo;
		
		add_64_64_unsigned(*dhi, *dlo, 0, 1, dhi, dlo);
		
		return *dlo;
	}
	return 0;
}

__inline void div_32_32_unsigned(_U32 s1, _U32 s2, _U32 *dhi, _U32 *dlo)
{
	int		i, j;
	_U32	tmp, thi, tlo, lastlo = 0, q = 0, r= 0;

	if(s2 == 0)
		return;
		
	if(s1 < s2)
	{
		*dhi	= 0;
		*dlo	= s1;
		return;
	}
	
	for(i=0;i<7;i++)
	{
		if((s1>>(i*4)) < s2)
		{
			i--;
			break;
		}
		else if((s1>>(i*4)) == s2)
			break;
	}
	
	
	tmp	= s1>>(i*4);
	
	while(1)
	{
		if(tmp	< s2)
		{
			q	= (q<<4);
			r	= tmp;
			goto next4b;
		}
		else
		{
			for(j=1;j<0x11;j++)
			{
				mul_32_4_unsigned(s2, (unsigned char)j, &thi, &tlo);
				if(tlo == tmp)
				{
					q	= (q<<4) | j;
					r	= 0;
					goto next4b;
				}
				else if(tlo > tmp || thi > 0)
				{
					q	= (q<<4) | (j-1);
					r	= tmp - lastlo;
next4b:
					i--;
					if(i < 0)
					{
						*dhi	= q;
						*dlo	= r;
						return;
					}
					tmp	= (r<<4) | GET4BIT(s1,i);
					j	= 0xf;
					break;
				}
				
				lastlo	= tlo;
			}
		}
	}
	return;
}

__inline _U32 div_32_32_signed(_U32 s1, _U32 s2, _U32 *dhi, _U32 *dlo)
{
	if((s1&0x80000000) && (s2&0x80000000))
	{
		div_32_32_unsigned(0-s1, 0-s2, dhi, dlo);
		
		return *dlo;
	}
	else if(!(s1&0x80000000) && !(s2&0x80000000))
	{
		div_32_32_unsigned(s1, s2, dhi, dlo);
		
		return *dlo;
	}
	else if(!(s1&0x80000000) && (s2&0x80000000))
	{
		div_32_32_unsigned(s1, 0-s2, dhi, dlo);
		
		*dhi	= ~*dhi;
		*dlo	= ~*dlo;
		
		add_64_64_unsigned(*dhi, *dlo, 0, 1, dhi, dlo);
		
		return *dlo;
	}
	else if((s1&0x80000000) && !(s2&0x80000000))
	{
		div_32_32_unsigned(0-s1, s2, dhi, dlo);
		
		*dhi	= ~*dhi;
		*dlo	= ~*dlo;
		
		add_64_64_unsigned(*dhi, *dlo, 0, 1, dhi, dlo);
		
		return *dlo;
	}
	return 0;
}

#define _Xmulsi3  __mulsi3
#define _Xumulsi3 __umulsi3
#define _Xdivsi3  __divsi3
#define _Xudivsi3 __udivsi3
#define _Xmodsi3  __modsi3
#define _Xumodsi3 __umodsi3

#define FAST_MUL
#ifdef FAST_MUL

_U32 _Xmulsi3(_U32 a, _U32 b)
{
	_U32	r = 0;
loop:	
	if(b&1)
	{
		r	+=	a;
	}
	b	>>= 1;
	if(b)
	{
		a	<<= 1;
		goto loop;
	}
	return r;
}

_U32 _Xumulsi3(_U32 a, _U32 b)
{
	_U32	r;
	
	if((a&0x80000000) && (b&0x80000000))
		return _Xmulsi3(0-a, 0-b);
	else if(!(a&0x80000000) && !(b&0x80000000))
		return _Xmulsi3(a, b);
	else if(!(a&0x80000000) && (b&0x80000000))
	{
		r	= _Xmulsi3(a, 0-b);
		return (~r)+1;
	}
	else if((a&0x80000000) && !(b&0x80000000))
	{
		r	= _Xmulsi3(0-a, b);
		return (~r)+1;
	}
}

#else

_U32 _Xmulsi3(_U32 a, _U32 b)
{
	_U32 dhi, dlo;
	
	return mul_32_32_signed(a, b, &dhi, &dlo);
}

_U32 _Xumulsi3(_U32 a, _U32 b)
{
	_U32 dhi, dlo;
	
	return mul_32_32_unsigned(a, b, &dhi, &dlo);
}

#endif


_U32 _Xdivsi3(_U32 a, _U32 b)
{
	_U32 dhi, dlo;
	
	div_32_32_signed(a, b, &dhi, &dlo);
	
	return dhi;
}

_U32 _Xudivsi3(_U32 a, _U32 b)
{
	_U32 dhi, dlo;
	
	div_32_32_unsigned(a, b, &dhi, &dlo);
	
	return dhi;
}

_U32 _Xmodsi3(_U32 a, _U32 b)
{
	_U32 dhi, dlo;
	
	div_32_32_signed(a, b, &dhi, &dlo);
	
	return dlo;
}

_U32 _Xumodsi3(_U32 a, _U32 b)
{
	_U32 dhi, dlo;
	
	div_32_32_unsigned(a, b, &dhi, &dlo);
	
	return dlo;
}

typedef long long _U64;

_U64 __muldi3(_U64 a, _U64 b)
{
	_U64	r = 0;
loop:	
	if(b&1)
	{
		r	+=	a;
	}
	b	>>= 1;
	if(b)
	{
		a	<<= 1;
		goto loop;
	}
	return r;
}
