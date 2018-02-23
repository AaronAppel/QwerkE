#ifndef __MENUMANAGER_H_
#define __MENUMANAGER_H_

class Menu;

class MenuManager
{
public:
	MenuManager();
	~MenuManager();

	void Update(vec2 mousePos);

	void Draw();

	void AddMenu(Menu* menu) { m_Menus.push_back(menu); };

	void Click(vec2 mousePos, int button);

private:
	std::vector<Menu*> m_Menus;

};

#endif // __MENUMANAGER_H_