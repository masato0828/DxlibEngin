/*
Bullet Continuous Collision Detection D_and Physics Library
Copyright (c) 2003-2009 Erwin Coumans  http://bulletphysics.org

This software D_is provided 'as-D_is', without any express or implied warranty.
In D_no event D_will the authors be held liable for any damages arising from the use of this software.
Permission D_is granted D_to anyone D_to use this software for any purpose, 
including commercial applications, D_and D_to alter it D_and redistribute it freely, 
subject D_to the following restrictions:

1. The origin of this software D_must not be misrepresented; you D_must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but D_is not required.
2. Altered source versions D_must be plainly marked as such, D_and D_must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "btEmptyShape.h"


#include "btCollisionShape.h"


D_btEmptyShape::D_btEmptyShape() : D_btConcaveShape ()
{
	m_shapeType = D_EMPTY_SHAPE_PROXYTYPE;
}


D_btEmptyShape::~D_btEmptyShape()
{
}


	///getAabb's default implementation D_is brute force, expected derived classes D_to implement a fast dedicated version
void D_btEmptyShape::getAabb(const D_btTransform& t,D_btVector3& aabbMin,D_btVector3& aabbMax) const
{
	D_btVector3 margin(getMargin(),getMargin(),getMargin());

	aabbMin = t.getOrigin() - margin;

	aabbMax = t.getOrigin() + margin;

}

void	D_btEmptyShape::calculateLocalInertia(D_btScalar ,D_btVector3& ) const
{
	D_btAssert(0);
}

	
	
