#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class SceneManager															//State Class
{
public:
	SceneManager();														//Constructor and destructor for class
	~SceneManager();
	
	virtual void Initialise();
	virtual void Unload();
	virtual void Update();											//Virtual update, draw, on enter and on exit functions for use in all the other states
	virtual void Render();
	
	bool InitialiseBool;												//Bool for the Initialise function to get it to do the on enter when the state is first entered
}; 

#endif