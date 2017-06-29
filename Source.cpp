/*
	author	: Muhammad Tashfiqul Bari
	website : www.selfloop.com 
*/

#include "iGame3D.h"

GameObject ourBox1,ourBox2,ourBox3;
float playerMovementSpeed = 0.25;
float playerForce = 100;

/* 
	function iDraw() is called again and again by the system.
*/

void iG3VariableDeclare(){
	
	ourBox1.rigidbody.ENABLE_RIGIDBODY();
	ourBox1.rigidbody.gravityScale = 0.1;
	ourBox1.rigidbody.linearDrag = 0.1;
	ourBox1.collider.SetCircleCollider(1,0,0);
	
	//ourBox2.collider.SetCircleCollider(1,0,1);
	ourBox2.collider.SetCircleCollider(1,0,0);
	//ourBox2.collider.SetCircleCollider(1,0,-1);
	//ourBox2.collider.SetCircleCollider(1,-1,0);
	//ourBox2.collider.SetCircleCollider(1,1,0);
}

void iG3Display(){
	
	ourBox1.CreateRectangle(0, 1, 1, 1);
	ourBox2.CreateRectangle(0, -2, 1, 1);

}

void iG3Keyboard(unsigned char key,int x,int y){
	
	if(key == 'w'){
	
		ourBox1.transform.translate(Vector2(0,playerMovementSpeed));
	}
	if(key == 's'){
	
		ourBox1.transform.translate(Vector2(0,-playerMovementSpeed));
	}
	if(key == 'a'){
	
		ourBox1.transform.translate(Vector2(-playerMovementSpeed,0));
	}
	if(key == 'd'){
	
		ourBox1.transform.translate(Vector2(playerMovementSpeed,0));
	}

	if(key == 'i'){
	
		ourBox1.rigidbody.AddForce(0,playerForce);
	}

	if(key == 'k'){
	
		ourBox1.rigidbody.AddForce(0,-playerForce);
	}

	if(key == 'j'){
	
		ourBox1.rigidbody.AddForce(-playerForce,0);
	}

	if(key == 'l'){
	
		ourBox1.rigidbody.AddForce(playerForce,0);
	}

}

void iG3Animation(){

}



int main(int argc,char **argv)
{
	//place your own initialization codes here. 
	glutInit(&argc,argv);
	DEBUG_MODE = true;
	GAME_MODE_3D = false;
	iG3VariableDeclare();
	iG3Initialization(720,480,"iGame3D");
	
	return 0;
}	