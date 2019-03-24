#ifndef _CAMERA_
#define _CAMERA_

#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define PI 3.1415

class Camera
{

public:

	float  x, y, z ;
	float  yaw ;   // angle between x and z 
	float  pitch  ;   // angle between y and z
  float  speed ;
	float  m_dirX ;//direction of camera
  float  m_dirZ ;//direction of camera
  float  m_dirY ;

	Camera()
	{
		//default positions
		x =0.0f ;
		y = 0.0f ; 
		z = 0.0f ;  
		yaw = 0.0f ;  
		pitch  = 0.0f ;
		speed = 5 ;//speed of camera
	    Update() ;	
	}

  inline Camera( float p_x, float p_y, float p_z,float p_lr, float p_ud, float p_spd )
	{
		Init( p_x, p_y, p_z, p_lr, p_ud, p_spd );
	}

	inline void SetPosition( float p_fPosX , float p_fPosY, float p_fPosZ )
	{
		x = p_fPosX ;
		y = p_fPosY ;
		z = p_fPosZ ;
	}

	inline void SetLeftRight( float p_fLR )
	{
		yaw = p_fLR ;
		Update() ;
	}

	inline void SetUpDown( float p_fUD )
	{
		pitch = p_fUD ;
	}

	inline void SetSpeed( float p_fSpd )
	{
		speed = p_fSpd  ;
	}

	inline void Init( float p_x, float p_y, float p_z, float p_lr, float p_ud, float p_spd )
	{
		SetPosition( p_x, p_y, p_z);
		SetLeftRight( p_lr );
		SetUpDown( p_ud );
		SetSpeed( p_spd );
		Update() ;
	}
	
	inline void Forward()
	{
        Update() ;

		x -=  m_dirX ;
		z -=  m_dirZ ;
        y +=  m_dirY ;

	}

	inline void Backward()
	{
        Update() ;

		x += m_dirX   ;
		z += m_dirZ   ;
		y -= m_dirY ;
	}

  inline void TurnRightLeft( float p_angle )
	{
		yaw = fmod(yaw + p_angle, 360); // (((int)yaw + (int)p_angle) % 360);
		
        Update() ;
		
	}
   
	inline void TurnUpDown( float p_angle )
	{
		
	  pitch = fmod(pitch + p_angle, 360);  // ( ((int) pitch  + (int) p_angle)  % 360 ) ;
	
	  if ( pitch >= 90 )
		  pitch = 89 ;

	  if ( pitch <= -90 )
		  pitch = -89 ;

	  
	  Update() ;
	}

  inline void Update()
	{

		m_dirX = speed * sin(yaw * PI / 180.0 )*cos(pitch*PI/180.0) ; 
		m_dirZ = speed * cos(yaw * PI / 180.0 )*cos(pitch*PI/180.0) ; 
        m_dirY = speed * sin(pitch*PI/180.0) ; 

	}

	inline glm::mat4 LookAt()
	{
		return glm::lookAt(glm::vec3(x, y, z), glm::vec3(x - m_dirX, y + m_dirY, z - m_dirZ), glm::vec3(0, 1, 0));

	}

	inline glm::vec3 position() {
		return glm::vec3(x, y, z);
	}
    
  inline float getLeftRight()
	{
		return yaw ;
	}

	inline void Print()
	{
		printf("Position [%.2f  %.2f  %.2f] Angles [%.1f %.1f]  Direction [%.2f  %.2f]\n",
			x, y, z, yaw, pitch,
			m_dirX, m_dirZ);
	}

	inline void Upward()
	{
		y += speed ;
	}

	inline void Downward()
	{
		y -= speed ;
	}

	inline void Left()
	{
       Update() ;

	   x -=  m_dirZ   ;
	   z +=  m_dirX ;
  }

	inline void Right()
	{
       Update() ;

	   x +=  m_dirZ   ;
	   z -=  m_dirX ;
  }

	inline float getX()
	{
		return x;
	}

	inline float getY()
	{
		return y;
	}

	inline float getZ()
	{
		return z;
	}

} ;

#endif