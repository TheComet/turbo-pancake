#include "Character.h"
#include "GSArena.h"

#define MAXHEALTH 10


Character::Character() : isdead(false),shoulddelete(false),
playercontrolled(false),
playerIsMovingUp(false),
playerIsMovingDown(false),
playerIsMovingRight(false),
playerIsMovingLeft(false),health(MAXHEALTH) { }

bool Character::isDead()  const { return isdead; }
bool Character::shouldDelete()  const { return shoulddelete;  }
void Character::setDead(bool arg) { isdead=arg;  }
void Character::setDelete(bool arg) { shoulddelete=arg; }
bool Character::isPlayerControlled() const { return playercontrolled; }
void Character::setPlayerControlled(bool arg) { playercontrolled=arg; }
void Character::kill() { setDead(true); setDelete(true); }

bool Character::dealDamage(double arg,Character *damageDealer,double invulnerabletime) {
    bool invulnerable=false;
    for (auto i=invulnerableList.begin();i!=invulnerableList.end();i++) {
        if ((*i).c==damageDealer) {
            invulnerable=true;
        }
    }
    if (!invulnerable) {
        health-=arg;
        invulnerableList.push_back({damageDealer,invulnerabletime});
    }
    return !invulnerable;
}
bool Character::isPushable() { return false; }

void Character::timestep(double dt,GSArena *gs) {
    if (isPlayerControlled()) {
        if (playerIsMovingUp)
            moveUp();
        if (playerIsMovingDown)
            moveDown();
        if(playerIsMovingRight)
            moveRight();
        if (playerIsMovingLeft)
            moveLeft();
        Vector2 fpos=gs->cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        lookAt(fpos.x,fpos.y,false);
    }
    auto i=invulnerableList.begin();
    while(i!=invulnerableList.end()) {
        (*i).t-=dt;
        if ((*i).t<0)
            i=invulnerableList.erase(i);
        else
            i++;
    }
}
void Character::handleEvent(SDL_Event *e,GSArena *gs) {
    if (e->type==SDL_KEYDOWN || e->type==SDL_KEYUP) {
        playerIsMovingLeft=g.adown;
        playerIsMovingRight=g.ddown;
        playerIsMovingUp=g.wdown;
        playerIsMovingDown=g.sdown;
    }
    else if (e->type==SDL_MOUSEMOTION) {
        Vector2 fpos=gs->cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        lookAt(fpos.x,fpos.y);
    }
    if (!g.mouseCapturedByGUI) {
        if (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_LEFT) {
            attack();
        }
        else if (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_RIGHT) {
            block();
        }
        else if (e->type==SDL_MOUSEBUTTONUP && e->button.button==SDL_BUTTON_LEFT) {

        }
        else if (e->type==SDL_MOUSEBUTTONUP && e->button.button==SDL_BUTTON_RIGHT) {
            idle();
        }
    }
}









Vector2 TestCharacter::getPos() {
    return pos;
}
void TestCharacter::setPos(Vector2 arg) {
    pos=arg;
}
bool TestCharacter::isPushable() {
    return true;
}
double TestCharacter::getCharacterRadius() {
    return characterRadius;
}

void TestCharacter::moveRight() {
    acc.x+=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveLeft() {
    acc.x-=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveUp() {
    acc.y-=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveDown(){
    acc.y+=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveDir(double x,double y) {
    acc=Vector2(x,y);
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::lookAt(double x,double y,bool showIcon) {
    targetAngle=atan2(y-pos.y,x-pos.x);
    if(showIcon)
        lastLookAt=SDL_GetTicks();
}

TestCharacter* TestCharacter::unsafe_copy() const{
    return new TestCharacter(*this);
}
TestCharacter::TestCharacter(double x,double y,float velcap,float acceleration,Texture img,Sound death) : Character(),
    //Texture initialization
    character(),sword(),shield(),directionIcon(),
    //sound initialization
    deathSound(),
    //movement initialization
    pos(),vel(),acc(),accMagnitude(0),speedcap(0),characterRadius(0),
    //angle initialization
    angle(0),targetAngle(0),lookSpeed(0),lastLookAt(0),
    //Attack initialization
    attackmode(AttackMode::idlestate),attackStarted(0),attackDuration(0),
    lastSwordBase(),lastSwordTip(),shieldangle(),swordangle(),
    shieldpos(),swordpos(),damage(1),hitAnimation(-1)
{
    //textures
    character=img;
    directionIcon=loadTexture("media/character-pointing.png");
    sword=loadTexture("media/basic_sword.png");
    shield=loadTexture("media/basic_shield.png");

    //sounds
    deathSound=death;

    //movement
    pos=Vector2(x,y);
    speedcap=velcap;
    accMagnitude=acceleration;
    characterRadius=0.47;

    //angle
    lookSpeed=20;

    //attack
    attackDuration=0.15;
    shieldpos=VectorEaser(Vector2(x,y),Vector2(x,y),40);
    swordpos=VectorEaser(Vector2(x,y),Vector2(x,y),40);
    shieldangle=AngleEaser(0,0,40);
    swordangle=AngleEaser(0,0,40);

}

bool TestCharacter::dealDamage(double arg,Character *damageDealer,double invulnerabletime) {
    bool ret=Character::dealDamage(arg,damageDealer, invulnerabletime);
    if (ret)
        hitAnimation=invulnerabletime;
    if (health<0)
        kill();
    else if(health<MAXHEALTH/2) {
        character.setColor(255,(Uint8)(health*255/(MAXHEALTH/2)),(Uint8)(health*255/(MAXHEALTH/2)));
    }
    return ret;
}
void TestCharacter::timestep(double dt, GSArena *gs) {
    Character::timestep(dt,gs); //handles player input if it is being received!!


    if (isDead() && !shouldDelete()) {
        gs->stickers.addSticker(g.bloodsplat,pos,0,1);
        shoulddelete=true;
        return;
    }

    vel+=acc*dt;
    //0 the vector if it gets too small.
    if (vel.length()<0.0005)
        vel=Vector2();
    //If velocity is greater than speedcap, set it to length speedcap.
    if (vel.length()>speedcap)
        vel=vel.normalized()*speedcap;

    //Apply friction.
    //This corresponds to viscous friction x''(t)=-friction*x'(t) instead of constant friction,
    //but we can tweak it if we want.
    double friction=5;
    if(acc.length2()<0.001)
        vel=vel*fmax(1-friction*dt,0);

    //after final adjustments, move the character's coordinates
    pos+=vel*dt;
    gs->map.circleMapCollide(pos,characterRadius);
    acc=Vector2(); //the character does not keep accelerating unless moveLeft is constantly called.


    //Super brute force "look at" method.
    //Calculates the distance between (cos(angle),sin(angle)) and (cos(targetAngle),sin(targetAngle)). 
    //if increasing angle decreases this distance, increase angle. Else decrease angle. And if 
    //they're close enough, just set angle=targetAngle.
    double c=cos(angle);
    double s=sin(angle);
    double ctarget=cos(targetAngle);
    double starget=sin(targetAngle);
    double c2=cos(angle+dt*lookSpeed);
    double s2=sin(angle+dt*lookSpeed);
    double d1=(c-ctarget)*(c-ctarget)+(s-starget)*(s-starget);
    double d2=(c2-ctarget)*(c2-ctarget)+(s2-starget)*(s2-starget);
    if (d1<0.1)
        angle=targetAngle;
    else if (d2<d1)
        angle+=dt*lookSpeed;
    else
        angle-=dt*lookSpeed;
    angle=fmod(angle,2*M_PI);

    if (attackmode==AttackMode::attackstate) {
        if (SDL_GetTicks()-attackStarted>(Uint32)(attackDuration*1000)) {
            attackmode=AttackMode::idlestate;
        }
    }

    double shieldAngleOffset=0,
        shieldRadius=0,
        swordAngleOffset=0,
        swordTargetAngle=0,
        swordRadius=0;
    /* Update sword and shield target positions. */
    if (attackmode==AttackMode::attackstate) {
        shieldAngleOffset=60.0*M_PI/180.0;
        shieldRadius=0.55;
        double Deltat=(SDL_GetTicks()-attackStarted)*0.001;
        swordAngleOffset=-60.0*M_PI/180.0 + Deltat/attackDuration*120*M_PI/180.0;
        swordTargetAngle=swordAngleOffset+M_PI/2.0;
        swordRadius=1;
    }
    else if (attackmode==AttackMode::blockstate) {
        shieldAngleOffset=0;
        shieldRadius=0.6;
        swordAngleOffset=-110.0*M_PI/180.0;
        swordTargetAngle=swordAngleOffset-M_PI/6.0;
        swordRadius=0.4;
    }
    else if (attackmode==AttackMode::idlestate) {
        shieldAngleOffset=60.0*M_PI/180.0;
        shieldRadius=0.6;
        swordAngleOffset=-60.0*M_PI/180.0;
        swordTargetAngle=swordAngleOffset;
        swordRadius=0.6;
    }
    double c1,s1;
    c1=cos(angle+shieldAngleOffset);
    s1=sin(angle+shieldAngleOffset);
    shieldpos.target=Vector2(c1,s1)*shieldRadius;
    shieldangle.target=shieldAngleOffset;
    c2=cos(angle+swordAngleOffset);
    s2=sin(angle+swordAngleOffset);
    swordpos.target=Vector2(c2,s2)*swordRadius;
    swordangle.target=swordTargetAngle;



    Vector2 nextSwordTip=pos+swordpos.point+0.42*Vector2(-sin(angle+swordangle.angle),cos(angle+swordangle.angle));
    Vector2 nextSwordBase=pos+swordpos.point- 0.29*Vector2(-sin(angle+swordangle.angle),cos(angle+swordangle.angle));

    if(attackmode==AttackMode::attackstate){
        swordquad=Quadrilateral(lastSwordBase,lastSwordTip,nextSwordTip,nextSwordBase);
        CharacterList &cr=*(gs->charman.getChars());
        for (auto i=cr.begin();i!=cr.end();i++) {
			Vector2 charpos = (*i)->getPos();
            if (doesCircleQuadCollide(charpos,(*i)->getCharacterRadius(),swordquad)) {
                if ((*i)!=this && (*i)->dealDamage(damage,this)) {
                    double x0 = rand() / (double)RAND_MAX -0.5;
                    double y0 = rand() / (double)RAND_MAX -0.5;
                    gs->plist.addTextParticle(charpos+Vector2(x0,y0),0.35,{200,0,0},"Ow!!!!!");
					//calculate direction to spray blood (just using beginning of sword tip for now, rather than checking for precise intersection
					Vector2 bloodDirection = (lastSwordTip - charpos).normalized();
					//just moving spawning of blood towards the outside of the character from the center
					charpos += (*i)->getCharacterRadius() * .8 * bloodDirection;
					gs->plist.addDirectionalBurst(charpos, atan2(-bloodDirection.y, bloodDirection.x), M_PI/12, 0.3f, 8, 10, 0.4f, "media/team2spawn.png", .05f);;

                }
            }
        }

    }
    hitAnimation-=dt;

    lastSwordTip=nextSwordTip;
    lastSwordBase=nextSwordBase;

    swordpos.easeexp(dt);
    shieldpos.easeexp(dt);
    swordangle.easeexp(dt);
    shieldangle.easeexp(dt);
}
void TestCharacter::attack() {
    if (attackmode==AttackMode::idlestate) {
        attackmode=AttackMode::attackstate;
        attackStarted=SDL_GetTicks();
    }
}
void TestCharacter::block() {
    if (attackmode==AttackMode::idlestate)
        attackmode=AttackMode::blockstate;

}
void TestCharacter::idle() {
    if (attackmode==AttackMode::blockstate)
        attackmode=AttackMode::idlestate;

}
void TestCharacter::kill() { setDead(true); }


void TestCharacter::render(const Camera& arg){
    double hitoffset=0;
    if(hitAnimation>0)
        hitoffset=sin(hitAnimation*20*M_PI)*0.04;
    arg.renderTexture(character,pos.x+hitoffset,pos.y,0,1);

    double timeSinceLook=(SDL_GetTicks()-lastLookAt)*0.001;
    double opacity=0.3;
    double startfade=0.2;
    double fadelength=0.4;
    if (timeSinceLook>startfade+fadelength) {
        opacity=0;
    }
    else if (timeSinceLook>startfade) {
        opacity=(1-(timeSinceLook-startfade)/fadelength)*0.3;
    }
    directionIcon.setAlpha((Uint8)(255*opacity));
    double iconRadius=0.6;
    arg.renderTexture(directionIcon,pos.x,pos.y,180.0*angle/M_PI,2);


    arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldangle.angle)/M_PI,0.6);
    arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,180.0*(angle+swordangle.angle)/M_PI,1);
    
    /*
    if (attackmode==AttackMode::attackstate) {
        double shieldoffset=60.0*M_PI/180.0;
        double shieldradius=0.55;
        double c1=cos(angle+shieldoffset);
        double s1=sin(angle+shieldoffset);
        arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldoffset)/M_PI,0.6);
        shieldpos.target=Vector2(c1,s1)*shieldradius;

        double Deltat=(SDL_GetTicks()-attackStarted)*0.001;
        double swordoffset=60.0*M_PI/180.0 - Deltat/attackDuration*120*M_PI/180.0;
        double swordradius=1;
        double c2=cos(angle-swordoffset);
        double s2=sin(angle-swordoffset);
        arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,90+180.0*(angle-swordoffset)/M_PI,1);
        swordpos.target=Vector2(c2,s2)*swordradius;
    }
    else if (attackmode==AttackMode::blockstate) {
        double shieldoffset=0;
        double shieldradius=0.6;
        double c1=cos(angle+shieldoffset);
        double s1=sin(angle+shieldoffset);
        arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldoffset)/M_PI,0.6);
        shieldpos.target=Vector2(c1,s1)*shieldradius;

        double swordoffset=110.0*M_PI/180.0;
        double swordradius=0.45;
        double c2=cos(angle-swordoffset);
        double s2=sin(angle-swordoffset);
        arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,30+90+180.0*(angle)/M_PI-180,1);
        swordpos.target=Vector2(c2,s2)*swordradius;
    }
    else if (attackmode==AttackMode::idlestate) {
        double shieldoffset=60.0*M_PI/180.0;
        double shieldradius=0.6;
        double c1=cos(angle+shieldoffset);
        double s1=sin(angle+shieldoffset);
        arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldoffset)/M_PI,0.6);
        shieldpos.target=Vector2(c1,s1)*shieldradius;

        double swordoffset=60.0*M_PI/180.0;
        double swordradius=0.6;
        double c2=cos(angle-swordoffset);
        double s2=sin(angle-swordoffset);
        arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,30+90+180.0*(angle)/M_PI,1);
        swordpos.target=Vector2(c2,s2)*swordradius;
    }*/

    swordquad.render(arg);
}



/*****************archer copy *******************************/



Vector2 Archer::getPos() {
	return pos;
}
void Archer::setPos(Vector2 arg) {
	pos = arg;
}
bool Archer::isPushable() {
	return true;
}
double Archer::getCharacterRadius() {
	return characterRadius;
}

void Archer::moveRight() {
	acc.x += accMagnitude;
	if (acc.length()>0)
		acc = accMagnitude*acc.normalized();
}
void Archer::moveLeft() {
	acc.x -= accMagnitude;
	if (acc.length()>0)
		acc = accMagnitude*acc.normalized();
}
void Archer::moveUp() {
	acc.y -= accMagnitude;
	if (acc.length()>0)
		acc = accMagnitude*acc.normalized();
}
void Archer::moveDown() {
	acc.y += accMagnitude;
	if (acc.length()>0)
		acc = accMagnitude*acc.normalized();
}
void Archer::moveDir(double x, double y) {
	acc = Vector2(x, y);
	if (acc.length()>0)
		acc = accMagnitude*acc.normalized();
}
void Archer::lookAt(double x, double y, bool showIcon) {
	targetAngle = atan2(y - pos.y, x - pos.x);
	if (showIcon)
		lastLookAt = SDL_GetTicks();
}

Archer* Archer::unsafe_copy() const {
	return new Archer(*this);
}
Archer::Archer(double x, double y, float velcap, float acceleration, Texture img, Sound death) : Character(),
//Texture initialization
character(), bow(), arrow(), directionIcon(),
//sound initialization
deathSound(),
//movement initialization
pos(), vel(), acc(), accMagnitude(0), speedcap(0), characterRadius(0),
//angle initialization
angle(0), targetAngle(0), lookSpeed(0), lastLookAt(0),
//Attack initialization
attackmode(AttackMode::idlestate), attackStarted(0), attackDuration(0),
lastSwordBase(), lastSwordTip(), bowangle(), arrowangle(),
shieldpos(), swordpos(), damage(1), hitAnimation(-1)
{
	//textures
	character = img;
	directionIcon = loadTexture("media/character-pointing.png");
	bow = loadTexture("media/basic_bow.png");
	arrow = loadTexture("media/basic_arrow.png");

	//sounds
	deathSound = death;

	//movement
	pos = Vector2(x, y);
	speedcap = velcap;
	accMagnitude = acceleration;
	characterRadius = 0.47;

	//angle
	lookSpeed = 20;

	//attack
	attackDuration = 0.15;
	shieldpos = VectorEaser(Vector2(x, y), Vector2(x, y), 40);
	swordpos = VectorEaser(Vector2(x, y), Vector2(x, y), 40);
	bowangle = AngleEaser(M_PI, 0, 40);
	arrowangle = AngleEaser(0, 0, 40);

}

bool Archer::dealDamage(double arg, Character *damageDealer, double invulnerabletime) {
	bool ret = Character::dealDamage(arg, damageDealer, invulnerabletime);
	if (ret)
		hitAnimation = invulnerabletime;
	if (health<0)
		kill();
	else if (health<MAXHEALTH / 2) {
		character.setColor(255, (Uint8)(health * 255 / (MAXHEALTH / 2)), (Uint8)(health * 255 / (MAXHEALTH / 2)));
	}
	return ret;
}
void Archer::timestep(double dt, GSArena *gs) {
	Character::timestep(dt, gs); //handles player input if it is being received!!


	if (isDead() && !shouldDelete()) {
		gs->stickers.addSticker(g.bloodsplat, pos, 0, 1);
		shoulddelete = true;
		return;
	}

	vel += acc*dt;
	//0 the vector if it gets too small.
	if (vel.length()<0.0005)
		vel = Vector2();
	//If velocity is greater than speedcap, set it to length speedcap.
	if (vel.length()>speedcap)
		vel = vel.normalized()*speedcap;

	//Apply friction.
	//This corresponds to viscous friction x''(t)=-friction*x'(t) instead of constant friction,
	//but we can tweak it if we want.
	double friction = 5;
	if (acc.length2()<0.001)
		vel = vel*fmax(1 - friction*dt, 0);

	//after final adjustments, move the character's coordinates
	pos += vel*dt;
	gs->map.circleMapCollide(pos, characterRadius);
	acc = Vector2(); //the character does not keep accelerating unless moveLeft is constantly called.


					 //Super brute force "look at" method.
					 //Calculates the distance between (cos(angle),sin(angle)) and (cos(targetAngle),sin(targetAngle)). 
					 //if increasing angle decreases this distance, increase angle. Else decrease angle. And if 
					 //they're close enough, just set angle=targetAngle.
	double c = cos(angle);
	double s = sin(angle);
	double ctarget = cos(targetAngle);
	double starget = sin(targetAngle);
	double c2 = cos(angle + dt*lookSpeed);
	double s2 = sin(angle + dt*lookSpeed);
	double d1 = (c - ctarget)*(c - ctarget) + (s - starget)*(s - starget);
	double d2 = (c2 - ctarget)*(c2 - ctarget) + (s2 - starget)*(s2 - starget);
	if (d1<0.1)
		angle = targetAngle;
	else if (d2<d1)
		angle += dt*lookSpeed;
	else
		angle -= dt*lookSpeed;
	angle = fmod(angle, 2 * M_PI);

	if (attackmode == AttackMode::attackstate) {
		if (SDL_GetTicks() - attackStarted>(Uint32)(attackDuration * 1000)) {
			attackmode = AttackMode::idlestate;
		}
	}

	double bowAngleOffset = 180,
		shieldRadius = 0,
		arrowAngleOffset = 0,
		swordTargetAngle = 0,
		swordRadius = 0;
	/* Update sword and shield target positions. */
	if (attackmode == AttackMode::attackstate) {
		bowAngleOffset = M_PI/2;
		shieldRadius = 0.55;
		double Deltat = (SDL_GetTicks() - attackStarted)*0.001;
		arrowAngleOffset = -60.0*M_PI / 180.0 + Deltat / attackDuration * 120 * M_PI / 180.0;
		swordTargetAngle = arrowAngleOffset + M_PI / 2.0;
		swordRadius = 1;
	}
	else if (attackmode == AttackMode::blockstate) {
		bowAngleOffset = -M_PI/9;
		shieldRadius = 0.6;
		arrowAngleOffset = -110.0*M_PI / 180.0;
		swordTargetAngle = arrowAngleOffset - M_PI / 6.0;
		swordRadius = 0.4;

		//just spawn an arrow for now
		Arrow a;
		a.pos = pos;
		a.vel = ARROW_SPEED * Vector2(cos(angle), sin(angle));
		arrowList.push_back(a);
	}
	else if (attackmode == AttackMode::idlestate) {
		bowAngleOffset = M_PI/2;
		shieldRadius = 0.6;
		arrowAngleOffset = -60.0*M_PI / 180.0;
		swordTargetAngle = arrowAngleOffset;
		swordRadius = 0.6;
	}
	double c1, s1;
	c1 = cos(angle + bowAngleOffset);
	s1 = sin(angle + bowAngleOffset);
	shieldpos.target = Vector2(c1, s1)*shieldRadius;
	bowangle.target = bowAngleOffset;
	c2 = cos(angle + arrowAngleOffset);
	s2 = sin(angle + arrowAngleOffset);
	swordpos.target = Vector2(c2, s2)*swordRadius;
	arrowangle.target = swordTargetAngle;

	//Arrow in flight management!
	auto it = arrowList.begin();
	while (it != arrowList.end()) {
		Vector2 flightoffset = it->vel*dt;
		it->pos += flightoffset;
		//move hitbox
		it->hitbox.p0 = it->pos;
		it->hitbox.p1 += flightoffset;
		it->hitbox.p2 += flightoffset;
		it->hitbox.p3 += flightoffset;

		//fetch characters
		CharacterList &cr = *(gs->charman.getChars());
		for (auto i = cr.begin(); i != cr.end(); i++) {
			//check collision
			Vector2 charpos = (*i)->getPos();
			if (doesCircleQuadCollide(charpos, (*i)->getCharacterRadius(), it->hitbox)) {
				if ((*i) != this && (*i)->dealDamage(damage, this)) {
					double x0 = rand() / (double)RAND_MAX - 0.5;
					double y0 = rand() / (double)RAND_MAX - 0.5;
					gs->plist.addTextParticle(charpos + Vector2(x0, y0), 0.35, { 200,0,0 }, "Ow!!!!!");
					Vector2 bloodDirection = -1 * it->vel.normalized();
					//just moving spawning of blood towards the outside of the character from the center
					charpos += (*i)->getCharacterRadius() * .8 * bloodDirection;
					gs->plist.addDirectionalBurst(charpos, atan2(-bloodDirection.y, bloodDirection.x), M_PI / 12, 0.3f, 6, 10, 0.3f, "media/team2spawn.png", .05f);;
					//arrow has hit! time to remove.
					it->canRemove = true;
					break;
				}
			}
		}
		if (it->canRemove)
			it = arrowList.erase(it);
		else
			++it;
	}



	Vector2 nextSwordTip = pos + swordpos.point + 0.42*Vector2(-sin(angle + arrowangle.angle), cos(angle + arrowangle.angle));
	Vector2 nextSwordBase = pos + swordpos.point - 0.29*Vector2(-sin(angle + arrowangle.angle), cos(angle + arrowangle.angle));

	if (attackmode == AttackMode::attackstate) {
		swordquad = Quadrilateral(lastSwordBase, lastSwordTip, nextSwordTip, nextSwordBase);
		CharacterList &cr = *(gs->charman.getChars());
		for (auto i = cr.begin(); i != cr.end(); i++) {
			Vector2 charpos = (*i)->getPos();
			if (doesCircleQuadCollide(charpos, (*i)->getCharacterRadius(), swordquad)) {
				if ((*i) != this && (*i)->dealDamage(damage, this)) {
					double x0 = rand() / (double)RAND_MAX - 0.5;
					double y0 = rand() / (double)RAND_MAX - 0.5;
					gs->plist.addTextParticle(charpos + Vector2(x0, y0), 0.35, { 200,0,0 }, "Ow!!!!!");
					//calculate direction to spray blood (just using beginning of sword tip for now, rather than checking for precise intersection
					Vector2 bloodDirection = (lastSwordTip - charpos).normalized();
					//just moving spawning of blood towards the outside of the character from the center
					charpos += (*i)->getCharacterRadius() * .8 * bloodDirection;
					gs->plist.addDirectionalBurst(charpos, atan2(-bloodDirection.y, bloodDirection.x), M_PI / 12, 0.3f, 8, 10, 0.4f, "media/team2spawn.png", .05f);;

				}
			}
		}

	}
	hitAnimation -= dt;

	lastSwordTip = nextSwordTip;
	lastSwordBase = nextSwordBase;

	swordpos.easeexp(dt);
	shieldpos.easeexp(dt);
	arrowangle.easeexp(dt);
	bowangle.easeexp(dt);
}
void Archer::attack() {
	if (attackmode == AttackMode::idlestate) {
		attackmode = AttackMode::attackstate;
		attackStarted = SDL_GetTicks();
	}
}
void Archer::block() {
	if (attackmode == AttackMode::idlestate)
		attackmode = AttackMode::blockstate;

}
void Archer::idle() {
	if (attackmode == AttackMode::blockstate)
		attackmode = AttackMode::idlestate;

}
void Archer::kill() { setDead(true); }


void Archer::render(const Camera& arg) {
	double hitoffset = 0;
	if (hitAnimation>0)
		hitoffset = sin(hitAnimation * 20 * M_PI)*0.04;
	arg.renderTexture(character, pos.x + hitoffset, pos.y, 0, 1);

	double timeSinceLook = (SDL_GetTicks() - lastLookAt)*0.001;
	double opacity = 0.3;
	double startfade = 0.2;
	double fadelength = 0.4;
	if (timeSinceLook>startfade + fadelength) {
		opacity = 0;
	}
	else if (timeSinceLook>startfade) {
		opacity = (1 - (timeSinceLook - startfade) / fadelength)*0.3;
	}
	directionIcon.setAlpha((Uint8)(255 * opacity));
	double iconRadius = 0.6;
	arg.renderTexture(directionIcon, pos.x, pos.y, 180.0*angle / M_PI, 2);


	arg.renderTexture(bow, pos.x + shieldpos.point.x, pos.y + shieldpos.point.y, 180.0*(angle + bowangle.angle) / M_PI, 1.3);
	arg.renderTexture(arrow, pos.x + swordpos.point.x, pos.y + swordpos.point.y, 180.0*(angle + arrowangle.angle) / M_PI, .7);

	//render flying arrows
	for(auto it = arrowList.begin(); it != arrowList.end(); ++it) {
		arg.renderTexture(it->t, it->pos.x, it->pos.y, 0, it->size);
	}

	swordquad.render(arg);
}



/*****************end archer copy ****************************************************/




//Deletes the memory and replaces list with an empty list.
void CharacterList::free() {
    for (auto i=list.begin(); i!=list.end();i++) {
        delete *i;
    }
    list=std::vector<Character *>();
}

//Returns a deep copy of list, by calling "new" a bunch of time, & Character's unsafe_copy function.
//Hands off the responsibility of deallocating memory to the caller.
std::vector<Character *> CharacterList::unsafe_copy() const {
    std::vector<Character *> ret;
    for (auto i=list.begin(); i!=list.end();i++) {
        ret.push_back((*i)->unsafe_copy());
    }
    return ret;
}
//empty constructor
CharacterList::CharacterList() : list() { }

//Rule of 3
CharacterList::CharacterList(const CharacterList &arg) : list() {
    list=arg.unsafe_copy();
}
CharacterList& CharacterList::operator=(const CharacterList &arg) {
    //Because we're calling new and delete here to make a deep copy,
    //we do have to check for self-assignment.
    if (&arg==this)
        return *this;

    //If we haven't been assigned to ourself, we can copy and free.
    free();
    list=arg.unsafe_copy();
    return *this;
}
CharacterList::~CharacterList() {
    free();
}

//std::vector-like stuff
//Accessors
Character& CharacterList::at(int i) {
    return *list[i];
}
Character& CharacterList::operator[](int i) {
    return *list[i];
}
//because size_t is overrated
int CharacterList::size() const {
    return (int)list.size();
}
//iterator stuff
CharacterList::iterator CharacterList::begin() {
    return list.begin();
}
CharacterList::iterator CharacterList::end() {
    return list.end();

}
void CharacterList::erase(iterator pt) {
    delete *pt;
    list.erase(pt);
}
void CharacterList::eraseNth(int n) {
    CharacterList::iterator pt=list.begin()+n;
    delete *pt;
    list.erase(pt);
}
void CharacterList::push_back(const Character& arg) {
    list.push_back(arg.unsafe_copy());
}
void CharacterList::clear() {
    free();
}