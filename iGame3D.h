#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glaux.lib")
#include "glut.h"
#include "glaux.h"
#include <time.h>
#include <math.h>
#include <windows.h>
#include "wtypes.h"

using namespace std;

bool _MoveInPhysicsEngine;

/*** (Begin) REFERENCE FOR MAIN_FUNCTION */
void iG3Display();
void iG3Animation();
void iG3Keyboard(unsigned char key,int x,int y);
/* End) -------------------------------- */

/*** (Begin) REFERENCE FOR iGame3D Function */
class Vector2;
class Vector3;
class Collider;
class Rigidbody;
class Transform;
class GameObject;

void iG3PhysicsEngineAsync();
void iG3PhysicsEngine();
void iG3Rectangle(Transform &_transform, double width, double height);
/* End) -------------------------------- */

/*** (Begin) Variables */
bool DEBUG_MODE = false;							// True : Enable Debug Mode
bool GAME_MODE_3D;									// True : Enable 3D Game Mode

static vector <GameObject> GAME_OBJECT_REFERENCE;

double cameraAngle;

int iG3ScreenWidth,iG3ScreenHeight;				// MonitorScreen (Width,Height)
int iG3WindowWidth,iG3WindowHeight;				// WindowScreen (Width,Height)
/*** (End) -------------------------------- */

/*** (iGame3D) : CUSTOM DATA TYPE */

class Vector3{

    public:

        double x;
        double y;
		double z;

		Vector3(){

			x = 0;
			y = 0;
			z = 0;
		}

        Vector3(double _x, double _y, double _z){

            x = _x;
            y = _y;
			z = _z;
        }

		// Magnitude of Vector
		static float magnitude(Vector3 v){

			return sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
		}

		 // Zero Vector
        static Vector3 zero(){

            return Vector3(0,0,0);
        }

        // Right Vector
        static Vector3 right(){

            return Vector3(1,0,0);
        }

        // Left Vector
        static Vector3 left(){

            return Vector3(-1,0,0);
        }

        // Up Vector
        static Vector3 up(){

            return Vector3(0,1,0);
        }

        // Down Vector
        static Vector3 down(){

            return Vector3(0,-1,0);
        }

		// Forwards Vector
        static Vector3 forwards(){

            return Vector3(0,0,1);
        }

        // Backwards Vector
        static Vector3 backwards(){

            return Vector3(0,0,-1);
        }

		// Vector : Add Operation
		static Vector3 add(Vector3 v1,Vector3 v2){

			return Vector3(v1.x + v2.x, v1.y + v2.y , v1.z + v2.z);
		}

		// Vector : Substract Operation
		static Vector3 substract(Vector3 v1, Vector3 v2){

			return Vector3(v1.x - v2.x, v1.y - v2.y , v1.z - v2.z);
		}

		// Vector : Multiply Operation
		static Vector3 multiply(Vector3 v, float multiplier){

			return Vector3( v.x * multiplier, v.y * multiplier , v.z * multiplier);
		}

		// Vector : distance
		static float Distance(Vector3 position_1, Vector3 position_2){

			return sqrt( pow( position_1.x - position_2.x,2 ) + pow( position_1.y - position_2.y,2 ) + pow( position_1.z - position_2.z,2 ) );
		}
};

class Vector2{

	private:

		double z;

    public:

		// Need To Hide From User :/
		double x;
        double y;

		Vector2(){

			x = 0;
			y = 0;
			z = 0;
		}

        Vector2(double _x, double _y){

            x = _x;
            y = _y;
			z = 0;
        }

		// Magnitude of Vector
		static float magnitude(Vector2 v){

			return Vector3::magnitude(Vector3(v.x,v.y,0));
		}

		 // Zero Vector
        static Vector3 zero(){

			return Vector3(0,0,0);
        }

        // Right Vector
        static Vector3 right(){

            return Vector3(1,0,0);
        }

        // Left Vector
        static Vector3 left(){

            return Vector3(-1,0,0);
        }

        // Up Vector
        static Vector3 up(){

            return Vector3(0,1,0);
        }

        // Down Vector
        static Vector3 down(){

            return Vector3(0,-1,0);
        }

		// Vector : Add Operation
		static Vector3 add(Vector2 v1,Vector2 v2){

			return Vector3::add(Vector3(v1.x, v1.y, 0),Vector3(v2.x, v2.y, 0));
		}

		// Vector : Substract Operation
		static Vector3 substract(Vector2 v1, Vector2 v2){

			return Vector3::substract(Vector3(v1.x, v1.y, 0),Vector3(v2.x, v2.y, 0));
		}

		// Vector : Multiply Operation
		static Vector3 multiply(Vector3 v, float multiplier){

			return Vector3::multiply(Vector3(v.x, v.y, 0), multiplier);
		}

		// Vector : distance
		static float Distance(Vector2 position_1, Vector2 position_2){

			return sqrt( pow( position_1.x - position_2.x,2 ) + pow( position_1.y - position_2.y,2 ) );
		}
};

class Collider{

	private:

		Vector3		_correctedTranslation;

		int		_numberOfBoxCollider;
		int		_numberOfCircleCollider;
		int		_numberOfSphereCollider;

		vector <Vector3>	_size;
		vector <float>		_radiusC;
		vector <float>		_radiusS;

		vector <Vector3>	_offset_BOX;
		vector <Vector3>	_offset_CIRCLE;
		vector <Vector3>	_offset_SPHERE;

	public:

		Collider(){

			_numberOfBoxCollider = 0;
			_numberOfCircleCollider = 0;
			_numberOfSphereCollider = 0;
		}
		//-------------------------------------------------------------------------------------------

		// Set 2D Collider : By (x,y)
		void SetBoxCollider(double x,double y,double sx,double sy){
			
			SetBoxCollider(Vector3(x,y,0),Vector3(sx,sy,0));
		}

		// Set 3D Collider : By (x,y,z)
		void SetBoxCollider(double x, double y, double z, double sx, double sy, double sz){
			
			SetBoxCollider(Vector3(x,y,z),Vector3(sx,sy,sz));
		}

		// Set 2D Collider : By Vector2
		void SetBoxCollider(Vector2 size,Vector2 offset){
		
			SetBoxCollider(Vector3(size.x,size.y,0),Vector3(offset.x,offset.y,0));
		}

		// Set 3D Collider By Vector3
		void SetBoxCollider(Vector3 size,Vector3 offset){

			_numberOfBoxCollider++;
			_size.push_back(size);
			_offset_BOX.push_back(offset);
		}

		void RemoveBoxCollider(){
		
		}
		
		//-------------------------------------------------------------------------------------------

		void SetCircleCollider(float radius, double x, double y){
			
			SetCircleCollider(radius,Vector2(x,y));
		}

		void SetCircleCollider(float radius,Vector2 offset){

			_numberOfCircleCollider++;
			_radiusC.push_back(radius/2);
			_offset_CIRCLE.push_back(Vector3(offset.x,offset.y,0));
		}

		//-------------------------------------------------------------------------------------------

		// Set Circle Collider
		void SetSphereCollider(float radius, double x, double y, double z){
		
			SetSphereCollider(radius,Vector3(x,y,z));
		}

		// Set Sphere Collider
		void SetSphereCollider(float radius, Vector3 offset){
		
			_numberOfSphereCollider++;
			_radiusS.push_back(radius);
			_offset_SPHERE.push_back(offset);
		}

		void RemoveCircleCollider(){
		
		}
		
		void ResetCollider(){
			
			RemoveBoxCollider();
			RemoveCircleCollider();
		}

		// ENGINE METHOD

		// Return : Number Of BoxCollider
		int GET_NUMBER_OF_BOX_COLLIDER(){
			
			return _numberOfBoxCollider;
		}

		// Return : Number Of CircleCollider
		int GET_NUMBER_OF_CIRCLE_COLLIDER(){
			
			return _numberOfCircleCollider;
		}

		// Return : Number of SphereCollider
		int GET_NUMBER_OF_SPHERE_COLLIDER(){
		
			return _numberOfSphereCollider;
		}

		// Get : Size of BoxCollider(index)
		Vector3 GET_SIZE_BOX_COLLIDER(int index){
			
			return _size[index];
		}

		// Get : Offset of BoxCollider(index)
		Vector3 GET_OFFSET_BOX_COLLIDER(int index){
			
			return _offset_BOX[index];
		}

		// Get : Radius of CircleCollider
		float GET_RADIUS_CIRCLE_COLLIDER(int index){
			
			return _radiusC[index];
		}

		// Get : Offset of CircleCollider
		Vector3 GET_OFFSET_CIRCLE_COLLIDER(int index){
			
			return _offset_CIRCLE[index];
		}

		// Get : Radius if SphereCollider
		float GET_RADIUS_SPHERE_COLLIDER(int index){
			
			return _radiusS[index];
		}

		// Get : Offset of SphereCollider
		Vector3 GET_OFFSET_SPHERE_COLLIDER(int index){
			
			return _offset_SPHERE[index];
		}

		void SET_CORRECTED_TRANSLATION(Vector3 correctedTranslation){
			
			_correctedTranslation = correctedTranslation;
		}

		Vector3 GET_CORRECTED_TRANSLATION(){
			
			return _correctedTranslation;
		}

};

class Rigidbody{

	private:

		Collider *_collider;

		bool _isMovementAllow;
		bool _isRigidbodyEnabled;

		// Gravity
		float _gravity;
		float _gravitationalForce;

		float getGravity(){

			_gravitationalForce += (_gravity - linearDrag);
			return (gravityScale/100) * _gravitationalForce;
		}

	public :

		float gravityScale;		// 9.8 x gravityScale( Multiplier )
		float linearDrag;		// Rate of deceleration for velocity
		float angulerDrag;		// Rate of rotation by force

		// Velocity
		Vector3 velocity;

		// Constructor
		Rigidbody(){

			_isMovementAllow	= false;
			_isRigidbodyEnabled = false;

			// Gravity : Default Value
			gravityScale = 1.0f;
			_gravity = 0.98;
			_gravitationalForce = 0.0f;

			// Velocity : Default Value
			linearDrag = 0.1;
		}

		Rigidbody(Collider &collider){

			_collider = &collider;

			_isRigidbodyEnabled = false;

			// Gravity : Default Value
			gravityScale = 1.0f;
			_gravity = 0.98;
			_gravitationalForce = 0.0f;

			// Velocity : Default Value
			linearDrag = 0.1;
		}

		void AddForce(double x,double y){

			AddForce(Vector3(x,y,0));
		}

		void AddForce(double x,double y,double z){

			AddForce(Vector3(x,y,z));
		}

		void AddForce(Vector2 forceOnDirection){

			AddForce(Vector3(forceOnDirection.x,forceOnDirection.y,0));
		}

		void AddForce(Vector3 forceOnDirection){

			velocity.x = (forceOnDirection.x/100);
			velocity.y = (forceOnDirection.y/100);
			velocity.z = (forceOnDirection.z/100);
		}

		// Enable : "RIGIDBODY" for the following "Game Object"
		void ENABLE_RIGIDBODY(){

			_isRigidbodyEnabled = true;
		}

		// Disable : "RIGIDBODY" for the following "Game Object"
		void DISABLE_RIGIDBODY(){

			_isRigidbodyEnabled = false;
		}

		// Get Status : Rigidbody Enabled/Disabled
		bool GET_STATUS_RIGIDBODY(){

			return _isRigidbodyEnabled;
		}

		void ALLOW_MOVEMENT(bool flag){
			
			_isMovementAllow = flag;
		}

		bool IS_MOVEMENT_ALLOW(){
			
			return _isMovementAllow;
		}

		Vector3 getForce(){

			if(velocity.x > 0){
				
				velocity.x -= linearDrag;
				if(velocity.x < 0)
					velocity.x = 0;
			}else if(velocity.x < 0){
				
				velocity.x += linearDrag;
				if(velocity.x > 0)
					velocity.x = 0;
			}

			velocity.y -= getGravity();

			if(velocity.y > 0)
				velocity.y -= linearDrag;
			else if(velocity.y < 0)
				velocity.y += linearDrag*velocity.y;
			

			return velocity;
		}

		void resetGravity(){
			
			_gravitationalForce = 0;
		}
};

class Transform{

	private:

		// Make Sure : The Reference of Collider & Rigidbody is set once.
		bool _isReferenceSet;

		Collider *_collider;		// Reference : GameObject -> Collider
		Rigidbody *_rigidbody;		// Reference : GameObject -> Rigidbody

		void _MoveTowards(Vector3 _targetedPosition,float _velocity){
			
			float _distance_X = abs(position.x - _targetedPosition.x);
			float _distance_Y = abs(position.y - _targetedPosition.y);
			float _distance_Z = abs(position.z - _targetedPosition.z);
			float _totalDistance = _distance_X + _distance_Y + _distance_Z;

			Vector3 _ratio = Vector3(_distance_X/_totalDistance,_distance_Y/_totalDistance,_distance_Z/_totalDistance);

			float _absoluteVelocity_X = _ratio.x * _velocity;
			float _absoluteVelocity_Y = _ratio.y * _velocity;
			float _absoluteVelocity_Z = _ratio.z * _velocity;

			if(_distance_X > _absoluteVelocity_X){

				if(_targetedPosition.x > position.x)
					position.x += _absoluteVelocity_X;
				else
					position.x -= _absoluteVelocity_X;
			}else
				position.x = _targetedPosition.x;


			if(_distance_Y > _absoluteVelocity_Y){

				if(_targetedPosition.y > position.y)
					position.y += _absoluteVelocity_Y;
				else
					position.y -= _absoluteVelocity_Y;
			}else
				position.y = _targetedPosition.y;

			if(_distance_Z > _absoluteVelocity_Z){

				if(_targetedPosition.z > position.z)
					position.z += _absoluteVelocity_Z;
				else
					position.z -= _absoluteVelocity_Z;
			}else
				position.z = _targetedPosition.z;
		}

	public:

		Vector3 _angle;
		Vector3 _translation;

		Vector3 position;
		Vector3 rotation;
		Vector3 scaling;

		Transform(){

			_translation = Vector3(0,0,0);
			_angle = Vector3(0,0,0);

			position = Vector3(0,0,0);
			rotation = Vector3(0,0,0);
			scaling = Vector3(1,1,1);
		}

		// CALL BY GAME_OBJECT, to set the reference
		Transform(Collider &collider,Rigidbody &rigidbody){

			if( _isReferenceSet != 1){

				_collider = &collider;
				_rigidbody = &rigidbody;

				_isReferenceSet = true;
			}

			_angle = Vector3(0,0,0);

			position = Vector3(0,0,0);
			rotation = Vector3(0,0,0);
			scaling = Vector3(1,1,1);

		}
		/*
			CALL BY USER: Transform.Translate
		*/

		// Parameter : Direction of Translation (x,y)
		void translate(double x,double y){

			translate(Vector3(x,y,0));
        }

		// Parameter : Direction of Translation (x,y,z)
		void translate(double x,double y,double z){

			translate(Vector3(x,y,z));
        }

		// Parameter : Direction of Translation (Vector2)
		void translate(Vector2 translate_direction){

			translate(Vector3(translate_direction.x,translate_direction.y,0));
		}

		// Parameter : Direction of Translation (Vector3)
		void translate(Vector3 translate_direction){

			_translation = translate_direction;

			if(_rigidbody->GET_STATUS_RIGIDBODY()){
				
				_MoveInPhysicsEngine = true;
				iG3PhysicsEngineAsync();
				//cout << _collider->GET_CORRECTED_TRANSLATION().x << "," << _collider->GET_CORRECTED_TRANSLATION().y << "," << _collider->GET_CORRECTED_TRANSLATION().z << endl;

				position.x += _collider->GET_CORRECTED_TRANSLATION().x;
				position.y += _collider->GET_CORRECTED_TRANSLATION().y;
				position.y += _collider->GET_CORRECTED_TRANSLATION().z;

				_collider->SET_CORRECTED_TRANSLATION(Vector3::zero());
			}else{
			
				position.x += _translation.x;
				position.y += _translation.y;
				position.y += _translation.z;

			}
			_translation = Vector3::zero();
			//cout << position.x << " : " << position.y << endl;
		}

		/*
			CALL BY USER: Transform.Rotate
		*/

		// Parameter : Rotation over AXIS (x,y,z)
		void rotate(double x, double y, double z){

			rotate(Vector3(x,y,z));
		}

		// Parameter : Rotation over AXIS (Vector3)
		void rotate(Vector3 _rotation){

			rotation = _rotation;

			if(_rotation.x > 0){

				_angle.x += _rotation.x;
				if( _angle.x > 360)
					_angle.x /= 360;

			}
			if(_rotation.y > 0){

				_angle.y += _rotation.y;
				if( _angle.y > 360)
					_angle.y /= 360;
			}
			if(_rotation.z > 0){

				_angle.z += _rotation.z;
				if( _angle.z > 360)
					_angle.z /= 360;
			}
}

		/*
			CALL BY USER: Transform.Scale
		*/

		// Parameter : Scaling over AXIS (x,y)
		void scale(double x, double y){

			scale(Vector3(x,y,0));
		}

		// Parameter : Scaling over AXIS (x,y,z)
		void scale(double x, double y, double z){

			scale(Vector3(x,y,z));
		}

		// Parameter : Scaling over AXIS (Vector2)
		void scale(Vector2 _scaling){

			scale(Vector3(_scaling.x,_scaling.y,0));
		}

		// Parameter : Scaling over AXIS (Vector3)
		void scale(Vector3 _scaling){

			scaling = _scaling;
		}

		/*
			CALL BY USER: Transform.Distance
		*/

		// Parameter : targetCordinate (x,y)
		float Distance(double x, double y){

			Distance(Vector3(x,y,0));
		}

		// Parameter : targetCordinate (x,y,z)
		float Distance(double x, double y, double z){

			Distance(Vector3(x,y,z));
		}

		// Parameter : targetCordinate (Vector2)
		float Distance(Vector2 _targetedPosition){

			Distance(Vector3(_targetedPosition.x,_targetedPosition.y,0));
		}

		// Parameter : targetCordinate (Vector3)
		float Distance(Vector3 _targetedPosition){

			return sqrt( pow( _targetedPosition.x - position.x,2 ) + pow( _targetedPosition.y - position.y,2 ) + pow( _targetedPosition.z - position.z,2 ) );
		}

		/*
			CALL BY USER: Transform.MoveTowards
		*/

		// Parameter : targetCordinate (x,y) , _velocity
		void MoveTowards(double _x, double _y, float _velocity){

			MoveTowards(Vector3(_x,_y,0),_velocity);
		}

		// Parameter : targetCordinate (x,y,z) , _velocity
		void MoveTowards(double _x, double _y, double _z, float _velocity){

			MoveTowards(Vector3(_x,_y,_z),_velocity);
		}

		// Parameter : targetCordinate (Vector2) , _velocity
		void MoveTowards(Vector2 _targetedPosition,float _velocity){

			MoveTowards(Vector3(_targetedPosition.x,_targetedPosition.y,0),_velocity);
		}

		// Parameter : targetCordinate (Vector3) , _velocity
		void MoveTowards(Vector3 _targetedPosition,float _velocity){

			if(_rigidbody->GET_STATUS_RIGIDBODY()){
			
				if(_rigidbody->IS_MOVEMENT_ALLOW()){
					
					_MoveTowards(_targetedPosition,_velocity);
				}
			}else{
			
				if(_rigidbody->IS_MOVEMENT_ALLOW()){
					
					_MoveTowards(_targetedPosition,_velocity);
				}
			}
		}
};

class GameObject{

	private:
		/*
		* _isCreated : Ensure that the configuretion are set once when its created
		* Other time, it will just only draw the "GameObject"
		*/
		bool _isCreated;
    public:

		Collider collider;					// User Reference	:	Collider
		Rigidbody rigidbody;				// User Reference	:	Rigidbody
		Transform transform;				// User Reference	:	Transform

		// Accesse By Llibrary It Self
		Collider	*_pointerOfCollider;	// Engine Reference	:	Collider
		Rigidbody	*_pointerOfRigidbody;	// Engine Reference	:	Rigidbody
		Transform	*_pointerOfTransform;	// Engine Reference	:	Transform

		// Constructor
        GameObject(){

			_isCreated = true;
			
			collider	=	Collider();
			rigidbody	=	Rigidbody( collider);
			transform	=	Transform( collider, rigidbody);

			_pointerOfCollider	=	&collider;
			_pointerOfRigidbody =	&rigidbody;
			_pointerOfTransform =	&transform;
			
			// Adding	:	The Following GameObject to Mother of GameObject
			GAME_OBJECT_REFERENCE.push_back(*this);
        }

		//------------------------------------------------------------------

		// Create GameObject : Position (X,Y) , width, height
		void CreateRectangle(double x,double y,double width, double height){

			CreateRectangle(Vector2(x,y),width,height);
        }

		// Create GameObject : Position (Vector2) , width, height
		void CreateRectangle(Vector2 _position2D,double width, double height){

			// When Its Created for the "First Time"
			if(_isCreated){

				// Storing	:	Position Reference
				transform.position.x = _position2D.x;
				transform.position.y = _position2D.y;

				// Storing	:	Dimension (width,height)
				transform.scaling.x = width;
				transform.scaling.y = height;
				transform.scaling.z = 0;

				// Make Sure : Its not getting accesse when its getting request to "Draw"
				_isCreated = false;
			}

			// Draw : Rectangle
			iG3Rectangle(transform, width, height);
        }

		// ------------------------------- CUSTOM MODEL -----------------------------------------------

		// Create Custom GameObject : Position (x,y), size (x,y), (func) CustomModel
		void CreateCustomModel( double x, double y, double sx, double sy,void (*CustomModel)(void)){

			CreateCustomModel(Vector3(x,y,0),Vector3(sx,sy,0), CustomModel);
		}

		// Create Custom GameObject : Position (x,y,z),size (x,y,y), (func) CustomModel
		void CreateCustomModel( double x, double y, double z, double sx, double sy, double sz, void (*CustomModel)(void)){

			CreateCustomModel(Vector3(x,y,z),Vector3(sx,sy,sz) ,CustomModel);
		}

		// Create Custom GameObject : Position (Vector2), size Vector2, (func) CustomModel
		void CreateCustomModel( Vector2 _position2D, Vector2 _size2D, void (*CustomModel)(void)){

			CreateCustomModel(Vector3(_position2D.x,_position2D.y,0),Vector3(_size2D.x,_size2D.y,0), CustomModel);
		}

		// Create Custom GameObject : Position (Vector3), size Vector3, (func) CustomModel
		void CreateCustomModel(Vector3 _position3D, Vector3 _size3D, void (*CustomModel)(void)){

			// When Its Created for the "First Time"
			if(_isCreated){

				// Storing	:	Position Reference
				transform.position.x = _position3D.x;
				transform.position.y = _position3D.y;
				transform.position.z = _position3D.z;

				// Storing	:	Dimension (width,height)
				transform.scaling.x = _size3D.x;
				transform.scaling.y = _size3D.y;
				transform.scaling.z = _size3D.z;

				// Make Sure : Its not getting accesse when its getting request to "Draw"
				_isCreated = false;
			}

			// Draw : Custom GameObject
			glPushMatrix();{

				glTranslatef(transform.position.x,transform.position.y,transform.position.z);
				glScalef(transform.scaling.x,transform.scaling.y,transform.scaling.z);
				CustomModel();
			}glPopMatrix();

		}
};

/* (End) ----------------- */

/*** (Begin) : Shape */

void iG3Line(double x1, double y1, double x2, double y2){

	glPushMatrix();{
		glBegin(GL_LINE_STRIP);
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		glEnd();
	}glPopMatrix();
}

void iG3Circle(double x, double y, double r, int slices=100){
	
	double t, PI=acosl(-1), dt, x1,y1, xp, yp;
	dt = 2*PI/slices;
	xp = x+r;
	yp = y;
	for(t = 0; t <= 2*PI; t+=dt)
	{
		x1 = x + r * cos(t);
		y1 = y + r * sin(t);
		iG3Line(xp, yp, x1, y1);
		xp = x1;
		yp = y1;
	}
}

void iG3Rectangle(Transform &_transform, double _width, double _height){

	glPushMatrix();{

			glColor3f(1,0,0);
			glTranslatef(_transform.position.x,_transform.position.y,_transform.position.z);
			glRotatef(_transform._angle.z,0,0,1);
			glScalef(_transform.scaling.x,_transform.scaling.y,0);
			glutSolidCube(1);
	}glPopMatrix();
}

/*** End) ----------------- */

void iG3PhysicsEngineAsync(){
	
	// Traversing GameObjects
	for(int i = 0 ; i < GAME_OBJECT_REFERENCE.size() ; i++){

		// COLLIDER
		if(GAME_OBJECT_REFERENCE[i]._pointerOfRigidbody->GET_STATUS_RIGIDBODY()){
			
			// Collider : Taking The Number Of Collider the following GameObject Has
			int _numberOfBoxCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_BOX_COLLIDER();
			int _numberOfCircleCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_CIRCLE_COLLIDER();
			int _numberOfSphereCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_SPHERE_COLLIDER();
		
			// Traversing CircleCollider
			for(int index = 0 ; index < _numberOfCircleCollider ; index++){
				
				Vector3 _position	= GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position;
				float	_radius		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_RADIUS_CIRCLE_COLLIDER(index);
				Vector3 _offset		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_OFFSET_CIRCLE_COLLIDER(index);

				// Traversing Other GameObject for collision
				for( int j = 0 ; j < GAME_OBJECT_REFERENCE.size()  ; j++){
				
					// If The Objects are not same
					if( j != i){
				
						int __numberOfCircleCollider	 = GAME_OBJECT_REFERENCE[j]._pointerOfCollider->GET_NUMBER_OF_CIRCLE_COLLIDER();

						// Traversing Their CircleCollider
						for(int _index = 0 ; _index < __numberOfCircleCollider ; _index++){

							Vector3 __position			=	GAME_OBJECT_REFERENCE[j]._pointerOfTransform->position;
							float	__radius			=	GAME_OBJECT_REFERENCE[j]._pointerOfCollider->GET_RADIUS_CIRCLE_COLLIDER(_index);
							Vector3 __offset			=	GAME_OBJECT_REFERENCE[j]._pointerOfCollider->GET_OFFSET_CIRCLE_COLLIDER(_index);

							//---------------------------
							// Collision by Translation
							//---------------------------
							float	_collsionDistance	= (_radius + __radius);
							Vector3	_requestedPosition	= Vector3::add(GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position,GAME_OBJECT_REFERENCE[i]._pointerOfTransform->_translation); 
							
							float	_distanceBeforeTranslation	= Vector3::Distance(_position,__position); 
							float	_distanceAfterTranslation	= Vector3::Distance(_requestedPosition,__position); 
							
							if(_collsionDistance > _distanceAfterTranslation){ 
								
								float _absDistanceToCoverPercentage =	abs(_distanceBeforeTranslation - _collsionDistance) / _distanceAfterTranslation;

								Vector3 __translation = GAME_OBJECT_REFERENCE[i]._pointerOfTransform->_translation; 
								__translation = Vector3(__translation.x * _absDistanceToCoverPercentage,__translation.y * _absDistanceToCoverPercentage,__translation.z * _absDistanceToCoverPercentage);
								GAME_OBJECT_REFERENCE[i]._pointerOfCollider->SET_CORRECTED_TRANSLATION(__translation);
							}else{
								
								GAME_OBJECT_REFERENCE[i]._pointerOfCollider->SET_CORRECTED_TRANSLATION(GAME_OBJECT_REFERENCE[i]._pointerOfTransform->_translation);
							}
						}
					}
				}
			}
		}
	}
}

void iG3PhysicsEngine(){
	
	while(true){
		
		// Traversing GameObjects
		for(int i = 0 ; i < GAME_OBJECT_REFERENCE.size() ; i++){

			// COLLIDER
			if(GAME_OBJECT_REFERENCE[i]._pointerOfRigidbody->GET_STATUS_RIGIDBODY()){
			
				// Collider : Taking The Number Of Collider the following GameObject Has
				int _numberOfBoxCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_BOX_COLLIDER();
				int _numberOfCircleCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_CIRCLE_COLLIDER();
				int _numberOfSphereCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_SPHERE_COLLIDER();
		
				// Traversing CircleCollider
				for(int index = 0 ; index < _numberOfCircleCollider ; index++){
				
					Vector3 _position	= GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position;
					float	_radius		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_RADIUS_CIRCLE_COLLIDER(index);
					Vector3 _offset		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_OFFSET_CIRCLE_COLLIDER(index);

					// Traversing Other GameObject for collision
					for( int j = 0 ; j < GAME_OBJECT_REFERENCE.size()  ; j++){
				
						// If The Objects are not same
						if( j != i){
				
							int __numberOfCircleCollider	 = GAME_OBJECT_REFERENCE[j]._pointerOfCollider->GET_NUMBER_OF_CIRCLE_COLLIDER();

							// Traversing Their CircleCollider
							for(int _index = 0 ; _index < __numberOfCircleCollider ; _index++){

								Vector3 __position			=	GAME_OBJECT_REFERENCE[j]._pointerOfTransform->position;
								float	__radius			=	GAME_OBJECT_REFERENCE[j]._pointerOfCollider->GET_RADIUS_CIRCLE_COLLIDER(_index);
								Vector3 __offset			=	GAME_OBJECT_REFERENCE[j]._pointerOfCollider->GET_OFFSET_CIRCLE_COLLIDER(_index);

								float	_collsionDistance	= (_radius + __radius);
								
								//--------------------------
								// Collision by Velocity
								//--------------------------

								Vector3 _velocity	=	GAME_OBJECT_REFERENCE[i]._pointerOfRigidbody->getForce();
								Vector3 _requestedPositionByVelocity	=	Vector3::add(_position,_velocity);
								float	_distanceBeforeForce			=	Vector3::Distance(_position,__position);
								float	_distanceAfterForce				=	Vector3::Distance(_requestedPositionByVelocity,__position);

								if(_collsionDistance > _distanceAfterForce){
								
									float _absDistanceToCoverPercentage	=	abs(_distanceBeforeForce - _collsionDistance) / _distanceAfterForce;
								
									GAME_OBJECT_REFERENCE[i]._pointerOfRigidbody->velocity = Vector3::zero();
									GAME_OBJECT_REFERENCE[i]._pointerOfRigidbody->resetGravity();
								}else{
									
									//cout << "Index : " << i << ", NCC : " << _numberOfCircleCollider << ", OtherIndex : " << j << ", ONCC : " << __numberOfCircleCollider << endl;
									//cout << _velocity.x << " , " << _velocity.y << " , "  << _velocity.z << endl;

									GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position.x += _velocity.x;
									GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position.y += _velocity.y;
									GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position.z += _velocity.z;
								}
							}
						}
					}
				}
			}
		}
		Sleep(60);
	}
}

/*** (Begin) : OpenGL Method */

void iG3Clear(){

	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void iG3MonitorInformation(){

	RECT _desktopMonitor;

	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &_desktopMonitor);

	iG3ScreenWidth = _desktopMonitor.right;
	iG3ScreenHeight = _desktopMonitor.bottom;
}

void iG3Camera(){

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(GAME_MODE_3D)
		gluLookAt(0*cos(cameraAngle), 15, 60,		0,0,0,		0,1,0);
	else
		gluLookAt(0, 0, 7.5,		0,0,0,		0,1,0);
	glMatrixMode(GL_MODELVIEW);
}

void iG3Axis(){

	glBegin(GL_LINES);
		//X axis
		glColor3f(1, 0, 0);	//100% Green
		glVertex3f(-150, 0, 0);
		glVertex3f( 150, 0, 0);

		//Y axis
		glColor3f(0, 1, 0);	//100% Blue
		glVertex3f(0, -150, 0);	// intentionally extended to -150 to 150, no big deal
		glVertex3f(0,  150, 0);

		//Z axis
		glColor3f(0, 0, 1);	//100% Blue
		glVertex3f( 0, 0, -150);
		glVertex3f(0, 0, 150);
	glEnd();

	if(GAME_MODE_3D){

		glColor3f(0.5, 0.5, 0.5);	//grey
		glBegin(GL_LINES);
			for(int i=-10;i<=10;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to X-axis
				glVertex3f(-100, 0, i*10);
				glVertex3f( 100, 0, i*10);

				//lines parallel to Z-axis
				glVertex3f(i*10, 0, -100);
				glVertex3f(i*10, 0, 100);
			}
		glEnd();
	}else{

		glColor3f(0.5, 0.5, 0.5);	//grey
		glBegin(GL_LINES);
			for(int i=-10;i<=10;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to X-axis
				glVertex3f(-100, i*10, 0);
				glVertex3f( 100, i*10, 0);

				//lines parallel to Y-axis
				glVertex3f(i*10, -100, 0);
				glVertex3f(i*10,  100, 0);
			}
		glEnd();
	}
}

void iG3DebugDisplay(){
	
	iG3Axis();

	// Traversing : GameObjects
	for(int i = 0 ; i < GAME_OBJECT_REFERENCE.size() ; i++){
			
		int _numberOfBoxCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_BOX_COLLIDER();
		int _numberOfCircleCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_CIRCLE_COLLIDER();
		int _numberOfSphereCollider	 = GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_NUMBER_OF_SPHERE_COLLIDER();
		
		// Traversing BoxCollider
		for(int index = 0 ; index < _numberOfBoxCollider ; index++){
				
			Vector3 _position	= GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position;
			Vector3 _size		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_SIZE_BOX_COLLIDER(index);
			Vector3 _offset		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_OFFSET_BOX_COLLIDER(index);

			glPushMatrix();{

				glColor3f(0,1,0);
				glTranslatef(_position.x,_position.y,_position.z);
				glScalef(_size.x,_size.y,_size.z);
				glutWireCube(1);
			}glPopMatrix();
		}

		// Traversing CircleCollider
		for(int index = 0 ; index < _numberOfCircleCollider ; index++){
			
			Vector3 _position	= GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position;
			float _radius		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_RADIUS_CIRCLE_COLLIDER(index);
			Vector3 _offset		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_OFFSET_CIRCLE_COLLIDER(index);

			glPushMatrix();{

				glColor3f(0,1,0);
				iG3Circle(_position.x+_offset.x,_position.y+_offset.y,_radius+_offset.z);
			}glPopMatrix();
		}

		// Traversing SphereCollider
		for(int index = 0 ; index < _numberOfSphereCollider ; index++){
			
			Vector3 _position	= GAME_OBJECT_REFERENCE[i]._pointerOfTransform->position;
			float _radius		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_RADIUS_SPHERE_COLLIDER(index);
			Vector3 _offset		= GAME_OBJECT_REFERENCE[i]._pointerOfCollider->GET_OFFSET_SPHERE_COLLIDER(index);

			glPushMatrix();{

				glColor3f(0,1,0);
				glTranslatef(_position.x,_position.y,_position.z);
				glRotatef(90,1,0,0);
				glutWireSphere(_radius,10,10);
			}glPopMatrix();
		}
	}
}

void iG3DisplayHandler(){

	iG3Clear();
	iG3Camera();

	if(DEBUG_MODE)
		iG3DebugDisplay();

	iG3Display();
	glutSwapBuffers();
}

void iG3AnimationHandler(){

	//cameraAngle += 0.001;
	iG3Animation();
	glutPostRedisplay();
}



void iG3Initialization(int _screenWidth,int _screenHeight,char *_gameTitle="iGame3D"){

	iG3WindowWidth = _screenWidth;
	iG3WindowHeight = _screenHeight;

	iG3MonitorInformation();
	glutInitWindowSize(_screenWidth, _screenHeight);
	glutInitWindowPosition( (iG3ScreenWidth/2) - (_screenWidth/2) , (iG3ScreenHeight/2) - (_screenHeight/2) );
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow(_gameTitle);

	cameraAngle = 0;
	glClearColor(0,0,0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70,	1,	0.1,	10000.0);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(iG3DisplayHandler);	//display callback function
	glutIdleFunc(iG3AnimationHandler);
	glutKeyboardFunc(iG3Keyboard);

	thread PhysicsEngine(iG3PhysicsEngine);
	PhysicsEngine.detach();

	glutMainLoop();
}

/*** End) ----------------- */
