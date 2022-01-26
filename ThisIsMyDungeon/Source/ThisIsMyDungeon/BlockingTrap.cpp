#include "BlockingTrap.h"

ABlockingTrap::ABlockingTrap()
{
	SetIsBlocking(true);
	hitBox->SetCollisionProfileName("BlockAllDynamicIgnoreCam", false);
	placeZone = PlaceZone::Ground;
}