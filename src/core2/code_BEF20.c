#include <ultra64.h>
#include "functions.h"
#include "variables.h"


f32 time_getDelta(void);
void func_80346DB4(s32);

s32  func_803463D4(enum item_e item, s32 diff);
void itemscore_noteScores_clear(void);
s32 itemscore_noteScores_get(enum level_e lvl_id);
void itemscore_timeScores_clear(void);

/* .bss */
s32 D_80385F30[0x2C];
s32 D_80385FE0;
s32 D_80385FE4;
s32 D_80385FE8;
f32 D_80385FEC;
u8  D_80385FF0[0xB];
f32 D_80386000[0xE]; //timescores
s32 D_80386038;

/* .code */
void func_80345EB0(enum item_e item){
    if(func_802FAFE8(item)){
        func_803463D4(item, (s32)(-time_getDelta()*60.0f * 1.1));
    }else{
        func_802FACA4(item);
    }
}

void item_inc(enum item_e item){
    func_803463D4(item, 1);
}

void item_dec(enum item_e item){
    if(!func_802E4A08())
        func_803463D4(item, -1);
}

s32 item_empty(enum item_e item){
    return (D_80385F30[item] != 0)? 0 : 1;
}

s32 item_getCount(enum item_e item){
    return D_80385F30[item];
}


#ifdef NONMATCHING
s32 func_80345FB4(enum item_e item, s32 diff, s32 arg2){\
    s32 oldVal;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24; //without this var newVal is too high, but sp1C is correct
    s32 newVal;


    oldVal = D_80385F30[item];

    if(func_80255D04())
        diff = 0;

    if(diff < 0){
        if( (item == ITEM_D_EGGS && func_803203FC(UNKFLAGS1_74_SANDCASTLE_INFINITE_EGGS))
            || (item == ITEM_16_LIFE && func_803203FC(UNKFLAGS1_73_SANDCASTLE_INFINITE_LIVES))
            || (item == ITEM_F_RED_FEATHER && func_803203FC(UNKFLAGS1_75_SANDCASTLE_INFINITE_RED_FEATHERS))
            || (item == ITEM_10_GOLD_FEATHER && func_803203FC(UNKFLAGS1_76_SANDCASTLE_INFINITE_GOLD_FEATHERS))
            || (item == ITEM_17_AIR && func_803203FC(UNKFLAGS1_96_SANDCASTLE_INFINITE_AIR))
        ){
            diff = 0;
        }
    }
    newVal = MAX(0, D_80385F30[item] + diff);
    D_80385F30[item] = newVal;

    sp34 = ((func_8031FF1C(BKPROG_B9_DOUBLE_HEALTH))? 2 : 1);
    D_80385F30[ITEM_15_HEALTH_TOTAL] = MIN(sp34*8, D_80385F30[ITEM_15_HEALTH_TOTAL]);
    D_80385F30[ITEM_14_HEALTH]= MIN(D_80385F30[ITEM_15_HEALTH_TOTAL], D_80385F30[ITEM_14_HEALTH]);
    D_80385F30[ITEM_17_AIR] = MIN(3600, D_80385F30[ITEM_17_AIR]);
    D_80385F30[ITEM_25_MUMBO_TOKEN_TOTAL] = D_80385F30[ITEM_1C_MUMBO_TOKEN];
    D_80385F30[ITEM_16_LIFE] = MIN(0xFF, D_80385F30[ITEM_16_LIFE]);

    switch(item){
        case ITEM_D_EGGS:
            sp38 = (func_8031FF1C(BKPROG_BE_CHEATO_BLUEEGGS))? 200 : 100;
            break;

        case ITEM_F_RED_FEATHER:
            sp38 = (func_8031FF1C(BKPROG_BF_CHEATO_REDFEATHERS))? 100 : 50;
            break;

        case ITEM_10_GOLD_FEATHER:
            sp38 = (func_8031FF1C(BKPROG_C0_CHEATO_GOLDFEATHERS))? 20 : 10;
            break;

        default:
            sp38 = 0;
            break;
    }
    if(sp38 != 0){
        D_80385F30[item] = MIN(sp38, D_80385F30[item]);
    }
    if(!arg2){
        func_802FACA4(item); //displays item on HUD
        if(item == ITEM_14_HEALTH || item == ITEM_17_AIR)
            func_802FACA4(ITEM_16_LIFE);
    }

    sp3C = item_empty(item);
    if(item < ITEM_6_HOURGLASS && sp3C)
        D_80385F30[item + ITEM_6_HOURGLASS] = FALSE;

    switch(item){
        case ITEM_14_HEALTH:
            if(sp3C)
                D_80385FE4 = 1;
            break;
        case ITEM_17_AIR:
            sp30 = func_80301D24(oldVal);
            sp2C = func_80301D24(newVal);
            if(sp3C){
                bs_checkInterrupt(BS_INTR_11_DROWN);
                D_80385FE4 = 1;
            }
            if(sp2C != 0 && sp30 != sp2C ){
                if(sp2C < sp30){
                    func_8025A6EC(SFX_AIR_METER_DROPPING, 28000);
                }
                else{
                    func_8030E760(SFX_3E9_UNKNOWN, 1.2f, 28000);
                }
            }
            break;
        case ITEM_C_NOTE:
            sp28 = itemscore_noteScores_get(level_get());
            func_80346DB4(D_80385F30[item]);
            if(D_80385F30[item] == 100 && sp28 != 100){
                func_8025A6EC(COMUSIC_36_100TH_NOTE_COLLECTED, 20000);
                item_inc(ITEM_16_LIFE);
            }
            break;
        case ITEM_26_JIGGY_TOTAL:
            D_80385F30[ITEM_2B_UNKNOWN] += diff;
            break;
    }
    return D_80385F30[item];
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/core2/code_BEF20/func_80345FB4.s")
#endif

s32 func_803463D4(enum item_e item, s32 diff){
    func_80345FB4(item, diff, 0);
}

void func_803463F4(enum item_e item, s32 diff){
    func_80345FB4(item, diff, 1);
}

void item_set(s32 item, s32 val){
    func_803463D4(item, val - item_getCount(item));
}

void func_80346448(s32 item){
    func_803463D4(item, 9999999);
}

void func_8034646C(void){
    int i;
    for(i = 0; i < 0x2C; i++){
        D_80385F30[i] = 0;
    }
    D_80385F30[ITEM_D_EGGS] = 0;
    D_80385F30[ITEM_F_RED_FEATHER] = 0;
    D_80385F30[ITEM_10_GOLD_FEATHER] = 0;
    D_80385F30[ITEM_14_HEALTH] = D_80385F30[ITEM_15_HEALTH_TOTAL] =  5;
    D_80385F30[ITEM_16_LIFE] = 3;
    D_80385F30[ITEM_17_AIR] = 3600;
    D_80385F30[ITEM_1C_MUMBO_TOKEN] = 0;
    D_80385F30[0x2B] = 0;
    D_80385F30[ITEM_26_JIGGY_TOTAL] = 0;
    itemscore_noteScores_clear();
    itemscore_timeScores_clear();
    D_80385FE4 = FALSE;
}

void func_803464F8(s32 level){
    int i;
    
    for(i = 0; i < 6; i++){
        D_80385F30[ITEM_0_HOURGLASS_TIMER + i] = 0;
        D_80385F30[ITEM_6_HOURGLASS + i] = 0;
    }
    
    D_80385F30[ITEM_C_NOTE] = 0;
    D_80385F30[ITEM_E_JIGGY] = jiggyscore_leveltotal(level);
    D_80385F30[ITEM_12_JINJOS] = 0;
    D_80385F30[ITEM_17_AIR] = 3600;
    D_80385F30[ITEM_18_GOLD_BULLIONS] = 0;
    D_80385F30[ITEM_19_ORANGE] = 0;
    D_80385F30[ITEM_23_ACORNS] = 0;
    D_80385F30[ITEM_1A_PLAYER_VILE_SCORE] = 0;
    D_80385F30[ITEM_1B_VILE_VILE_SCORE] = 0;
    D_80385F30[ITEM_1F_GREEN_PRESENT] = 0;
    D_80385F30[ITEM_20_BLUE_PRESENT] = 0;
    D_80385F30[ITEM_21_RED_PRESENT] = 0;
    D_80385F30[ITEM_22_CATERPILLAR] = 0;
    func_802FA5D0();
    D_80385FE8 = 1;
}

void func_803465BC(void){
    func_802FBDFC();
}

void func_803465DC(void){}

void func_803465E4(void){
    s32 sp54 = bs_getState();
    s32 sp50 = 0;
    s32 sp4C = level_get();
    s32 i;
    int is_underwater;
    int is_on_water_surface;
    int is_in_polluted_or_winter_water;

    if(func_80334904() != 2) return;
    if(D_80385FE8){
        if( func_802BC248() != 3
            && func_8028F070()
            && map_get() != MAP_33_UNUSED
            && map_get() != MAP_91_FILE_SELECT
        ){
            D_80385FE0 = TRUE;
        }//L80346674
        D_80385FE8 = FALSE;
    }//L8034667C
    
    if(D_80385FE0){
        if(gctransition_8030BD98() || func_803203FC(0)){
            if(D_80385FE4){
                item_dec(ITEM_16_LIFE);
                func_802FACA4(ITEM_14_HEALTH);
            }
            D_80385FE4 = FALSE;
            sp50 = TRUE;
            D_80385FE0 = FALSE;
        }
    }
    else{//L803466E8
        if( D_80385F30[ITEM_14_HEALTH] != 0
            && D_80385F30[ITEM_14_HEALTH] < 3
            && func_80347A4C()
        ){
            sp50 = TRUE;
        }
    }//L80346710

    if(sp50){
        func_803463D4(ITEM_14_HEALTH, 0);
    }

    if(sp54 == BS_24_FLY || sp54 == BS_23_FLY_ENTER){
        func_803463D4(ITEM_F_RED_FEATHER, 0);
    }

    if(!func_8028EC04() && func_8028F070()){
        if(level_get() != LEVEL_2_TREASURE_TROVE_COVE || !levelSpecificFlags_get(5)){
            is_underwater = (func_8028EE84() == BSWATERGROUP_2_UNDERWATER);
            is_on_water_surface = (func_8028EE84() == BSWATERGROUP_1_SURFACE);
            is_in_polluted_or_winter_water = ((level_get() == LEVEL_9_RUSTY_BUCKET_BAY) || (map_get() == MAP_46_CCW_WINTER));
            if( is_in_polluted_or_winter_water && (is_underwater || is_on_water_surface)){ //L803467EC
                D_80385FEC = 2.0f;
            }
            else{//L80346804
                D_80385FEC = MAX(0.0, D_80385FEC - time_getDelta());
            }//L80346870
            if( (!is_in_polluted_or_winter_water && is_underwater) || (is_in_polluted_or_winter_water && is_on_water_surface) ){//L80346894
                func_803463D4(ITEM_17_AIR, (s32)((f64)((-time_getDelta())*60.0f)*1.1));
            }
            else{ 
                if(is_in_polluted_or_winter_water && is_underwater){//L803468D8
                    func_803463D4(ITEM_17_AIR, (s32)(f64)((-time_getDelta()*60.0f)*2.1));
                }//L80346930
                if(!is_in_polluted_or_winter_water || D_80385FEC == 0.0f){
                    if(!D_80385FE4 && D_80385F30[ITEM_17_AIR] < 3600){
                        func_803463D4(ITEM_17_AIR, (s32)(((time_getDelta()*60.0f)*100.0)*1.1));
                        D_80385F30[ITEM_17_AIR] = MIN(D_80385F30[ITEM_17_AIR], 3600);
                    }
                }
            }
        }//L803469E4
    }//L803469E4

    if(!func_803203FC(0xbf)){
        for(i = 0; i < 6; i++){
            if(D_80385F30[ITEM_6_HOURGLASS + i]){
                func_80345EB0(ITEM_0_HOURGLASS_TIMER + i);
            }
        }
    }//L80346A2C

    if( getGameMode() != GAME_MODE_4_PAUSED
        && func_8028F070()
    ){
        if(sp4C == 0xC)
            sp4C = 6;
        if(sp4C > 0  && sp4C < 0xC && map_get() != MAP_91_FILE_SELECT){
            D_80386000[sp4C] = MAX(1.0, MIN(65535.0, D_80386000[sp4C] + time_getDelta()));
        }
    }//L80346B6C

    if((func_8023DB5C() & 7) == 6){
        if(!func_80320708() || !func_80320248()){
            D_80385F30[randi2(0, 0x2C)] = 1;
            D_80385FF0[randi2(0, 0xE)] = 1;
            D_80386000[randi2(0, 0xE)] = 1.0f;
        }
    }
}

void func_80346C10(enum bs_e *retVal, enum bs_e fail_state, enum bs_e success_state, enum item_e item_id, int use_item){
    if(item_empty(item_id)){
        func_803463D4(item_id, 0);
        func_8025A6EC(COMUSIC_2C_BUZZER, 22000);
        if(fail_state != -1){
            *retVal = fail_state;
        }
    }
    else{
        if(use_item){
            item_dec(item_id);
        }
        if(success_state != -1)
            *retVal = success_state;
    }
}

void func_80346CA8(void) {
    D_80385FE0 = 0;
    if (D_80385FE4) {
        D_80385FE0 = TRUE;
        D_80385F30[ITEM_14_HEALTH] = D_80385F30[ITEM_15_HEALTH_TOTAL];
        D_80385F30[ITEM_17_AIR] = 60*60;
    }
}

void func_80346CE8(void){
    D_80385FE4 = FALSE;
}

enum item_e carriedobj_actorId2ItemId(enum actor_e actor_id){
    switch(actor_id){
        case ACTOR_29_ORANGE_COLLECTABLE:           return ITEM_19_ORANGE;
        case ACTOR_2A9_ACORN:                       return ITEM_23_ACORNS;
        case ACTOR_2A2_CATERPILLAR:                 return ITEM_22_CATERPILLAR;
        case ACTOR_2A_GOLD_BULLION:                 return ITEM_18_GOLD_BULLIONS;
        case ACTOR_1ED_BLUE_PRESENT_COLLECTABLE:    return ITEM_20_BLUE_PRESENT;
        case ACTOR_1EF_GREEN_PRESENT_COLLECTABLE:   return ITEM_1F_GREEN_PRESENT;
        case ACTOR_1F1_RED_PRESENT_COLLECTABLE:     return ITEM_21_RED_PRESENT;
    }
    return 0;
}

void itemscore_noteScores_clear(void) {
    s32 i;

    for(i = 0; i < 0xe; i++){
        D_80385FF0[i] = 0;
    }
}

//itemscore_noteScores_update
void func_80346DB4(s32 note_count) {
    s32 level_id;

    level_id = level_get();
    if (!func_802E4A08() && (level_id > 0) && (level_id < 0xE)) {
        if (D_80385FF0[level_id] < note_count) {
            D_80385FF0[level_id] = note_count;
            if ((level_get() == LEVEL_1_MUMBOS_MOUNTAIN) && (note_count == 50)) {
                func_80311480(0xF74, 4, NULL, NULL, NULL, NULL);
            }
            if (note_count == 100) {
                func_80311480(0xF78, 4, NULL, NULL, NULL, NULL);
            }
            if (note_count == 1) {
                levelSpecificFlags_set(0x34, TRUE);
            }
            if (!levelSpecificFlags_get(0x34) && (func_80311480(0xF76, 0, NULL, NULL, NULL, NULL))) {
                levelSpecificFlags_set(0x34, TRUE);
            }
            if (func_803203FC(0x17) == 0) {
                func_803204E4(0x17, 1);
                func_80320524(0x19, level_id, 4);
            }
        }
    }
}

s32 itemscore_noteScores_getTotal(void){
    int i = 1;
    s32 total = 0;
    do{
        total += D_80385FF0[i++];
    }while(i < 0xe);
    return total;
}

s32 itemscore_noteScores_get(enum level_e lvl_id){
    return D_80385FF0[lvl_id];
}

void func_80346F44(s32 *size, void **ptr) {
    static u64 D_80386040;
    s32 var_s0;

    *size = sizeof(u64);
    *ptr = (void *)&D_80386040;
    D_80386040 = 0;
    for(var_s0 = 1; var_s0 < 0xB; var_s0++){
        if(var_s0 != 6){
            D_80386040 <<= 7;
            D_80386040 |= D_80385FF0[var_s0];
        }
    }
}

void itemscore_noteScoress_maxAll(void) {
    s32 i;

    for(i = 1; i < 11; i++){
        D_80385FF0[i] = 100;
    }
}

void itemscore_timeScores_clear(void) {
    s32 i;
    for(i = 0; i < 0xE; i++){
        D_80386000[i] = 0.0f;
    }
}

s32 itemscore_timeScores_getTotal(void) {
    s32 total;
    s32 i;

    total = 0;
    for(i = 0; i < 0xE; i++){
        total += D_80386000[i];
    }
    return total;
}

u16 itemscore_timeScores_get(enum level_e level_id) {
    return  (u16) D_80386000[level_id];
}

void itemscore_timeScores_getSizeAndPtr(s32 *size, void **ptr) {
    static u16 D_80386048[0xB]; //timescores_truncated
    s32 i;

    *size = 0xB*sizeof(s16);
    for(i = 0; i < 0xB; i++){
        D_80386048[i] = (u16) D_80386000[i + 1];

    }
    *ptr = (void *)&D_80386048;
}

//itemscore_getSavedItemArray
void func_80347630(s32 *size, u8 **buffer){
    static u8 D_80386060[5]; //saved item array

    D_80386060[0] = item_getCount(ITEM_1C_MUMBO_TOKEN);
    D_80386060[1] = item_getCount(ITEM_D_EGGS);
    D_80386060[2] = item_getCount(ITEM_F_RED_FEATHER);
    D_80386060[3] = item_getCount(ITEM_10_GOLD_FEATHER);
    D_80386060[4] = item_getCount(ITEM_26_JIGGY_TOTAL);
    *size = 5;
    *buffer = D_80386060;
}

void itemscore_highNoteScores_fromSaveData(u8 *savedata) {
    u64 sp28;
    s32 temp_ret;
    s32 i;
    enum level_e level_id;

    sp28 = *(u64*)savedata;
    level_id = 10;
    for( i = 0; i != 9; i++){
        D_80385FF0[level_id] = sp28 & 0x7F;
        level_id--;
        sp28 >>= 7;
        
        if (level_id == LEVEL_6_LAIR) {
            level_id--;
        }
        
    }
}

void itemscore_timeScores_fromSaveData(u16 *savedata) {
    s32 i;

    itemscore_timeScores_clear();
    for(i = 0; i < 0xB; i++){
        D_80386000[i + 1] = savedata[i];
    }
}

void func_8034789C(void) {
    s32 sp1C;
    s32 temp_v0;

    sp1C = honeycombscore_get_total();
    D_80385F30[ITEM_13_EMPTY_HONEYCOMB] = sp1C % 6;
    if (func_8031FF1C(BKPROG_B9_DOUBLE_HEALTH)) {
        D_80385F30[ITEM_15_HEALTH_TOTAL] = 16;
    } else {
        D_80385F30[ITEM_15_HEALTH_TOTAL] =  5 + MIN(3, (sp1C / 6));
    }
    if (func_803203FC(0x94)) {
        temp_v0 = D_80385F30[ITEM_15_HEALTH_TOTAL];
        if (temp_v0 >= 9) {
            D_80385F30[ITEM_15_HEALTH_TOTAL] = temp_v0;
        }
        else{
            D_80385F30[ITEM_15_HEALTH_TOTAL] = 8;
        }
    }
}

void func_80347958(void){
    func_8034789C();
    D_80385F30[ITEM_14_HEALTH] = D_80385F30[ITEM_15_HEALTH_TOTAL];
}

void func_80347984(void){}

void func_8034798C(void){
    D_80385F30[ITEM_E_JIGGY] = jiggyscore_leveltotal(level_get());
    func_802FA5D0();
}

//restore item counts from savestate
void func_803479C0(u8 *arg0){
    D_80385F30[ITEM_1C_MUMBO_TOKEN] = D_80385F30[ITEM_25_MUMBO_TOKEN_TOTAL] = arg0[0];
    D_80385F30[ITEM_D_EGGS] = arg0[1];
    D_80385F30[ITEM_F_RED_FEATHER] = arg0[2];
    D_80385F30[ITEM_10_GOLD_FEATHER] = arg0[3];
    D_80385F30[ITEM_26_JIGGY_TOTAL] = D_80385F30[ITEM_2B_UNKNOWN] = arg0[4];
    func_802FA5D0();
}


void func_80347A14(s32 arg0){
    if(arg0){
        D_80386038--;
    }
    else{
        D_80386038++;
    }
}

bool func_80347A4C(void){
    return (D_80386038 != 0) ? FALSE : TRUE;
}

void func_80347A70(void){
    D_80386038 = 0;
}

s32 D_80386068;

void func_80347A7C(void){
    func_80320748();
    D_80386068 = item_getCount(ITEM_16_LIFE);
}

void func_80347AA8(void) {
    func_80320798();
    func_803204E4(0x1F, FALSE);
    func_803204E4(0x20, FALSE);
    func_803204E4(0xC1, FALSE);
    func_803204E4(0xC0, FALSE);
    item_set(ITEM_16_LIFE, D_80386068);
    func_802FA5D0();
}
