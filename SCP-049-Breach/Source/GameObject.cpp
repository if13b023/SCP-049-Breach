#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(int id)
{
	m_id = id;
}


GameObject::~GameObject()
{
}

int GameObject::ID()
{
	return m_id;
}
