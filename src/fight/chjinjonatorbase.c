#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "fight.h"

typedef struct {
    u8 unk0[4];
}ActorLocal_fight_7BE0;

void chjinjonatorbase_update(Actor *);

/* .data */
ActorInfo chJinjonatorBase = {
    MARKER_27F_JINJONATOR_STATUE_BASE, ACTOR_3A9_JINJONATOR_STATUE_BASE, ASSET_544_MODEL_JINJONATOR_STATUE_BASE,
    0x1, NULL,
    chjinjonatorbase_update, func_80326224, actor_draw,
    0, 0x800, 0.0f, 0
};

s32 D_80391A64[3] = {0xC8, 0xC8, 0xA0};

ParticleScaleAndLifetimeRanges fight_D_80391A70 = {
    {1.0f, 1.0f},
    {1.7f, 2.7f},
    {0.0f, 0.05f},
    {2.6f, 3.4f},
    0.0f, 0.1f
};

struct42s D_80391A98 = {
    { {-170.0f, 0.0f, -170.0f}, {170.0f, 100.0f, 170.0f}},
    { {-90.0f, 0.0f, -80.0f},
      {80.0f, 60.0f, 80.0f}
    }
};

/* .code */ 
void chjinjonatorbase_spawnStoneJinjo(ActorMarker *marker) {
    Actor *this;
    Actor *stone_jinjo;

    this = marker_getActor(marker);
    stone_jinjo = spawn_child_actor(ACTOR_3A1_STONE_JINJO, &this);
    stone_jinjo->unkF4_8 = this->unkF4_8;
    stone_jinjo->position_y = stone_jinjo->position_y + 155.0f;
    this->unk100 = stone_jinjo->marker;
}

void func_8038E03C(f32 arg0[3], u32 arg1) {
    ParticleEmitter *temp_v0 = partEmitMgr_newEmitter(arg1);

    particleEmitter_setSprite(temp_v0, ASSET_70E_SPRITE_SMOKE_2);
    particleEmitter_setRGB(temp_v0, D_80391A64);
    particleEmitter_setStartingFrameRange(temp_v0, 0, 7);
    particleEmitter_setPosition(temp_v0, arg0);
    particleEmitter_setPositionAndVelocityRanges(temp_v0, &D_80391A98);
    particleEmitter_setScaleAndLifetimeRanges(temp_v0, &fight_D_80391A70);
    particleEmitter_emitN(temp_v0, arg1);
}

void func_8038E0D4(Actor *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 temp_v0 = func_8034C2C4(arg0->marker, arg1);

    if (temp_v0) {
        func_8034DE60(temp_v0, arg2, arg3, arg4, 1);
    }
}

void func_8038E120(ActorMarker * marker, ActorMarker *arg1){
    Actor *actor = marker_getActor(marker);
    ActorLocal_fight_7BE0 * local = (ActorLocal_fight_7BE0 *)&actor->local;
    int indx;
    s32 remaining;
    f32 pad;
    

    indx = marker->unk40_31 - 1;
    if(actor->state != 3){
        if(local->unk0[indx]){
            local->unk0[indx]--;
            comusic_playTrack(COMUSIC_2B_DING_B);
            if(local->unk0[indx] <= 0){
                func_8038E0D4(actor, indx + 0x19a, -100.0f, 0.0f, 1.2f);
                func_80324D54(1.2f, SFX_90_SWITCH_PRESS, 1.0f, 32000, actor->position, 1000.0f, 2000.0f);
            }
        }//L8038E210
        remaining = local->unk0[0] + local->unk0[1] + local->unk0[2] + local->unk0[3];
        if( remaining <= 0){
            func_8038C148();
            subaddie_set_state(actor, 3);
            chstonejinjo_breakOpen(actor->unk100);
        }
    }//L8038E24C
}


void func_8038E260(ActorMarker *arg0) {
    Actor *sp24 = marker_getActor(arg0);

    func_802BB3DC(0, 18.0f, 0.92f);
    __spawnQueue_add_4((GenFunction_4) func_802C4140, 0x3AD, *(u32 *)(&sp24->unk1C_x), *(u32 *)(&sp24->unk1C_y), *(u32 *)(&sp24->unk1C_z));
    func_8038E03C(sp24->unk1C, 0x10);
}

void func_8038E2CC(ActorMarker *arg0) {
    func_8032BB88(marker_getActor(arg0), -1, 0x7FFF);
}

void chjinjonatorbase_update(Actor *this){
    ActorLocal_fight_7BE0 *local = (ActorLocal_fight_7BE0 *)&this->local;
    f32 sp58 = time_getDelta();
    s32 temp_s1 = globalTimer_getTime() & 0xf;
    s32 temp_s0;
    int i;
    Actor *other;
    f32 temp_f20;
    f32 sp38[3];
    

    marker_loadModelBin(this->marker);
    if(!this->volatile_initialized){
        this->volatile_initialized = TRUE;
        marker_setCollisionScripts(this->marker, NULL, func_8038E120, NULL);
        this->marker->propPtr->unk8_3 = 1;
        actor_collisionOn(this);
        for(i = 0; i < 4; i++){
            local->unk0[i] = 5;
            func_8038E0D4(this, 0x19a + i, 0.0f, -300.0f, 0.0f);
        }
        this->unk44_31 = func_8030D90C();
        sfxsource_setSfxId(this->unk44_31, 0x3f9);
        func_8030DD14(this->unk44_31, 3);
        func_8030DBB4(this->unk44_31, 1.0f);
        sfxsource_setSampleRate(this->unk44_31, 26000);
        func_8030E2C4(this->unk44_31);
        temp_s0 = func_802F9AA8(0x3EC);
        func_802F9DB8(temp_s0, 0.5f, 0.5f, 0.0f);
        func_802F9EC4(temp_s0, this->position, 5000, 15000);
        func_802F9FD0(temp_s0, 0.25f, this->lifetime_value - 1.0, 1);
        func_802FA060(temp_s0, 17000, 17000, 0.0f);
        func_8025A6EC(JINGLE_MENACING_GRUNTILDA_A, 15000);
        func_8032BB88(this, 0, 0x7fff);
        timedFunc_set_1(8.0f, (GenFunction_1)func_8038E2CC, reinterpret_cast(s32,this->marker));
        this->unk1C[0] = this->position_x;
        this->unk1C[1] = this->position_y;
        this->unk1C[2] = this->position_z;
        this->position_y = -600.0f;
        FUNC_8030E624(SFX_3F6_UNKNOWN, 0.6f, 25000);
        if(0.0f != this->lifetime_value){
            this->velocity_y = 600.0f/this->lifetime_value;
        }else{
            this->velocity_y = 100.0f;
        }
        timedFunc_set_1(this->lifetime_value* 0.05, (GenFunction_1)func_8038E260,  reinterpret_cast(s32,this->marker));
        timedFunc_set_1(this->lifetime_value* 0.28, (GenFunction_1) func_8038E260, reinterpret_cast(s32,this->marker));
        timedFunc_set_1(this->lifetime_value* 0.46, (GenFunction_1) func_8038E260, reinterpret_cast(s32,this->marker));
        timedFunc_set_1(this->lifetime_value* 0.58, (GenFunction_1) func_8038E260, reinterpret_cast(s32,this->marker));
        SPAWNQUEUE_ADD_1(chjinjonatorbase_spawnStoneJinjo, this->marker);
        
    }
    else{//L8038E5CC
        if(this->state == 1){
            other = marker_getActor(this->unk100);
            temp_f20 = this->velocity_y * sp58;
            if(this->position_y + temp_f20 < this->unk1C[1]){
                if(!func_8030E3FC(this->unk44_31)){
                    func_8030E2C4(this->unk44_31);
                }

                sp38[0] = (temp_s1 & 1) ? 3.0f : -3.0f;
                sp38[1] = temp_f20;
                sp38[2] = (temp_s1 & 2) ? 3.0f : -3.0f;

                this->position_x = this->unk1C[0];
                this->position_z = this->unk1C[2];

                this->position_x = sp38[0] + this->position_x;
                this->position_y = sp38[1] + this->position_y;
                this->position_z = sp38[2] + this->position_z;

                other->position_x = this->unk1C[0];
                other->position_z = this->unk1C[2];

                other->position_x = sp38[0] + other->position_x;
                other->position_y = sp38[1] + other->position_y;
                other->position_z = sp38[2] + other->position_z;
            }
            else{//L8038E6FC
                subaddie_set_state(this, 2);
                FUNC_8030E624(SFX_7F_HEAVYDOOR_SLAM, 0.8f, 32000);
                func_8030E394(this->unk44_31);
                func_8030DA44(this->unk44_31);
                this->unk44_31 = 0;
                this->position_x = this->unk1C[0];
                this->position_y = this->unk1C[1];
                this->position_z = this->unk1C[2];

                other->position_x = this->unk1C[0];
                other->position_y = this->unk1C[1];
                other->position_z = this->unk1C[2];

                other->position_y += 155.0f;

            }
        }
    }//L8038E768
}


