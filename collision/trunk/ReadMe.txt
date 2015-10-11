This library provides a framework for implementing simple rigid body collision detection, suitable for games without the need for realistic dynamics. Several predefined collision shapes are available, and the system can be extended to add more if desired.

CollisionObject is an abstract superclass that represents a single collidable object in a simulation. A typical CollisionObject subclass will include an updatePosition() method and a position ivar. updatePosition() should be called once per frame to specify the intended trajectory of the object, and the position ivar can be read after collision resolution to learn the new position after trajectory has been corrected for collisions.

For each CollisionObject, you must provide a collision response callback capable of correcting the object's trajectory on collision so that the same collision will not occur on the next iteration of CollisionResolver_resolveAll(). Collision response, dynamics, and physical properties are outside the scope of this library, and are the sole responbility of CollisionCallback implementations.

CollisionObject's shapeType ivar is used to identify both the specific subclass of shape, and the intersection handler necessary for testing against other CollisionObjects. For each unique pair of CollisionObject shapeTypes (including a pair of the same shapeType), an IntersectionHandler must exist if they are to be collidable. The IntersectionHandler's responsibility is to determine whether two objects with given trajectories would collide, and if so, return details about the collision such as the exact time at which collision would take place and the normal vector of the collision. IntersectionHandlers are provided for all included shapeType pairs (or at least, they will be at version 1.x and above; currently only a subset are implemented). The caller is responsible for providing appropriate IntersectionHandlers for any additional shapeTypes they create.

CollisionResolver manages the details of using IntersectionHandlers to detect collisions between pairs of CollisionObjects in its list, and calling their CollisionCallback to resolve those collisions. The resolveAll() method works iteratively to attempt to detect and resolve all collisions. The findEarliest() and querySingle() methods can be used for speculative intersection testing without calling any CollisionCallbacks to resolve collisions.


Usage example:

CollisionResolver * collisionResolver;
CollisionCapsule * playerObject;
CollisionBox3D * obstacleObject;
CollisionTrimesh * levelObject;

void init() {
	collisionResolver = CollisionResolver_create(IntersectionManager_createWithStandardHandlers(), true);
	
	playerObject = CollisionCapsule_create(playerModel, playerCollisionCallback, PLAYER_INITIAL_POSITION, PLAYER_RADIUS, PLAYER_HEIGHT);
	obstacleObject = CollisionBox3D_create(...);
	levelObject = CollisionTrimesh_create(...);
	CollisionResolver_addObject(collisionResolver, playerObject);
	CollisionResolver_addObject(collisionResolver, obstacleObject);
	CollisionResolver_addObject(collisionResolver, levelObject);
}

void update() {
	// ...Update playerModel->velocity based on inputs, gravity, etc...
	CollisionObject_updatePosition(playerObject, Vector3x_add(playerObject->position, playerModel->velocity));
	CollisionResolver_resolveAll(collisionResolver);
}

void playerCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize) {
	CollisionCapsule * capsule = (CollisionCapsule *) collision.object1;
	PlayerModel * playerModel = (PlayerModel *) capsule->owner;
	
	// Bounce off object that was collided with so that collision stops happening
	playerModel->velocity = Vector3x_reflect(playerModel->velocity, collision.normal);
	
	// Don't call updatePosition inside CollisionCallback; modify the position ivar directly instead so that lastPosition is unchanged
	capsule->position = Vector3x_add(capsule->lastPosition, Vector2x_multiplyScalar(ball->velocity, timesliceSize));
}

// TODO: Example of CollisionObject_setMasks()
