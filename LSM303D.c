#ifndef __LSM303Dc__
	
	#define __LSM303Dc__
	void vector_normalize_E(void){
		float ax;
		ax=sqrt((E.x*E.x) + (E.y*E.y) + (E.z*E.z));
		E.x /= ax;
		E.y /= ax;
		E.z /= ax;
	}
	
	void vector_normalize_N(void){
		float ax;
		ax=sqrt((N.x*N.x) + (N.y*N.y) + (N.z*N.z));
		N.x /= ax;
		N.y /= ax;
		N.z /= ax;
	}
	float vector_dot_E(void){
		return (E.x * from.x) + (E.y * from.y) + (E.z * from.z);
	}
	
	float vector_dot_N(void){
		return (N.x * from.x) + (N.y * from.y) + (N.z * from.z);
	}
	
	void configure_LSM303D(void){
		delay_ms(100);
		i2c_start();
		aux=i2c_write(LSM303_WRITE);
		if(aux==0){
			//Configuramos Ctrl2
			i2c_write(0x21);//whoami
			i2c_write(0x00);
		}else{
			heading = 500.0;
			return;
		}	
		i2c_stop();
		
		delay_ms(100);
		i2c_start();
		aux=i2c_write(LSM303_WRITE);
		if(aux==0){
			//Configuramos Ctrl1
			i2c_write(0x20);//whoami
			i2c_write(0x57);
		}else{
			heading = 500.0;
			return;
		}	
		i2c_stop();
		
		delay_ms(100);
		i2c_start();
		aux=i2c_write(LSM303_WRITE);
		if(aux==0){
			//Configuramos Ctrl5
			i2c_write(0x24);
			i2c_write(0x64);
		}else{
			heading = 500.0;
			return;
		}	
		i2c_stop();
		
		delay_ms(100);
		i2c_start();
		aux=i2c_write(LSM303_WRITE);
		if(aux==0){
			//Configuramos Ctrl6
			i2c_write(0x25);
			i2c_write(0x20);
		}else{
			heading = 500.0;
			return;
		}	
		i2c_stop();
		
		delay_ms(100);
		i2c_start();
		aux=i2c_write(LSM303_WRITE);
		if(aux==0){
			//Configuramos Ctrl7
			i2c_write(0x26);
			i2c_write(0x00);
		}else{
			heading = 500.0;
			return;
		}	
		i2c_stop();
	}
	
	void get_heading(void){
		char aux,aux2;
				
		//Leemos magnetómetro
		aux2=0x08;
		i2c_start();
		aux=i2c_write(LSM303_write);
		if(aux==0){
			i2c_write(aux2|0x80);
			i2c_start();
			aux=i2c_write(LSM303_read);
			if(aux==0){
				//Leemos X
				aux=i2c_read();
				aux2=i2c_read();
				m.x=((int16)aux2<<8) | aux;
				//Leemos Y
				aux=i2c_read();
				aux2=i2c_read();
				m.y=((int16)aux2<<8) | aux;
				//Leemos Z
				aux=i2c_read();
				aux2=i2c_read();
				m.z=((int16)aux2<<8) | aux;
				i2c_read(0);
			}else{
				heading = 500.0;
				return;
			}
		}else{
			output_high(PIN_D4);
			heading = 500.0;
			return;
		}
				
		i2c_stop();
		
		aux2=0x28;
		i2c_start();
		aux=i2c_write(LSM303_write);
		if(aux==0){
			i2c_write(aux2|0x80);
			i2c_start();
			aux=i2c_write(LSM303_read);
			if(aux==0){
				//Leemos X
				aux=i2c_read();
				aux2=i2c_read();
				a.x=((int16)aux2<<8) | aux;
				//Leemos Y
				aux=i2c_read();
				aux2=i2c_read();
				a.y=((int16)aux2<<8) | aux;
				//Leemos Z
				aux=i2c_read();
				aux2=i2c_read();
				a.z=((int16)aux2<<8) | aux;
				i2c_read(0);
			}else{
				heading = 500.0;
				return;
			}
		}else{
			heading = 500.0;
			return;
		}
				
			
		i2c_stop();
		
		//Calculamos el heading
		 temp_m.x=m.x;
		 temp_m.y=m.y;
		 temp_m.z=m.z;
		 
		 //producto cruz de E = temp_m X a
		 E.x = (temp_m.y * a.z) - (temp_m.z * a.y);
		 E.y = (temp_m.z * a.x) - (temp_m.x * a.z);
		 E.z = (temp_m.x * a.y) - (temp_m.y * a.x);
		 
		 //Normalización de E
		 vector_normalize_E();
		 
		//producto cruz de N = a X E
		 N.x = (a.y * E.z) - (a.z * E.y);
		 N.y = (a.z * E.x) - (a.x * E.z);
		 N.z = (a.x * E.y) - (a.y * E.x);
		 
		 //Normalización de N
		 vector_normalize_N();
		 
		 //Estas variables se usan depende la ortientacion de la tarjeta respecto a la horizontal
		 from.x=1;
		 from.y=0;
		 from.z=0;
		 
		 heading = (atan2(vector_dot_E(),vector_dot_N()) * 180) / 3.14159264 ;
		 
		 if(heading <0) heading += 360;
		 		
	}
#endif