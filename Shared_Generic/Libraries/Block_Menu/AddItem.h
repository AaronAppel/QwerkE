#ifndef __AddItem_H_
#define __AddItem_H_

class Mesh;

class AddButton : public MenuItem
{
public:
	AddButton(Mesh* mesh);
	~AddButton();

	void Draw();
	void LeftClick();

private:

};

#endif // __AddItem_H_