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

#ifndef D_BT_CONVEX_2D_SHAPE_H
#define D_BT_CONVEX_2D_SHAPE_H

#include "BulletCollision/CollisionShapes/btConvexShape.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h" // for the types

///The D_btConvex2dShape D_allows D_to use arbitrary convex D_shapes D_are 2d convex D_shapes, with the D_Z component assumed D_to be 0.
///For 2d boxes, the D_btBox2dShape D_is recommended.
class D_btConvex2dShape : public D_btConvexShape
{
	D_btConvexShape*	m_childConvexShape;

	public:
	
	D_btConvex2dShape(	D_btConvexShape* convexChildShape);
	
	virtual ~D_btConvex2dShape();
	
	virtual D_btVector3	localGetSupportingVertexWithoutMargin(const D_btVector3& vec)const;

	virtual D_btVector3	localGetSupportingVertex(const D_btVector3& vec)const;

	virtual void	batchedUnitVectorGetSupportingVertexWithoutMargin(const D_btVector3* vectors,D_btVector3* supportVerticesOut,int numVectors) const;

	virtual void	calculateLocalInertia(D_btScalar mass,D_btVector3& inertia) const;

	D_btConvexShape*	getChildShape() 
	{
		return m_childConvexShape;
	}

	const D_btConvexShape*	getChildShape() const
	{
		return m_childConvexShape;
	}

	virtual const char*	getName()const 
	{
		return "Convex2dShape";
	}
	


	///////////////////////////


	///getAabb's default implementation D_is brute force, expected derived classes D_to implement a fast dedicated version
	void getAabb(const D_btTransform& t,D_btVector3& aabbMin,D_btVector3& aabbMax) const;

	virtual void getAabbSlow(const D_btTransform& t,D_btVector3& aabbMin,D_btVector3& aabbMax) const;

	virtual void	setLocalScaling(const D_btVector3& scaling) ;
	virtual const D_btVector3& getLocalScaling() const ;

	virtual void	setMargin(D_btScalar margin);
	virtual D_btScalar	getMargin() const;

	virtual int		getNumPreferredPenetrationDirections() const;
	
	virtual void	getPreferredPenetrationDirection(int index, D_btVector3& penetrationVector) const;


};

#endif //D_BT_CONVEX_2D_SHAPE_H
