#ifndef __LSM303Dh__
	
	#define __LSM303Dh__
	
	#define LSM303_READ	0x3B
	#define LSM303_WRITE 0x3A
	
	struct _vector{
		signed int16 x;
		signed int16 y;
		signed int16 z;
	};
	
	struct _vector32{
		signed int32 x;	
		signed int32 y;
		signed int32 z;
	};
	
	struct _float{
		float x;
		float y;
		float z;
	};
	
	struct _float E, N;
	struct _vector32 temp_m;
	struct _vector a,m,from;
	float heading;//En esta variable almacenamos el heading cada vez que se actualiza. Si heading es igual a 500 significa error
	
	void vector_normalize_E(void);
	void vector_normalize_N(void);
	float vector_dot_E(void);
	float vector_dot_N(void);
	void get_heading(void);//Actualiza el heading
	void configure_LSM303D(void);//Mandamos llamar esta rutina para configurar los parámetros del sensor
	
#endif