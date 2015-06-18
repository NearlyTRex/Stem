Usage example:

CollisionResolver * collisionResolver;
IntersectionManager * intersectionManager;
CollisionCapsule * playerObject;
CollisionBox * obstacleObject;
CollisionTrimesh * levelObject;

void init() {
	intersectionManager = IntersectionManager_createWithStandardHandlers();
	collisionResolver = CollisionResolver_create(intersectionManager, true, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	
	playerObject = CollisionCapsule_create(...);
	obstacleObject = CollisionBox_create(...);
	levelObject = CollisionTrimesh_create(...);
	CollisionResolver_addObject(collisionResolver, playerObject);
	CollisionResolver_addObject(collisionResolver, obstacleObject);
	CollisionResolver_addObject(collisionResolver, levelObject);
}

void update() {
	CollisionObject_updatePosition(playerObject, playerPosition);
	CollisionResolver_resolveAll(collisionResolver);
}
