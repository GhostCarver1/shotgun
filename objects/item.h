#ifndef ITEM_H
#define ITEM_H

typedef enum {
    ITEM_MAGNIFYING_GLASS,
    ITEM_HANDCUFFS,
    ITEM_BEER,
    ITEM_CIGARETTE_PACK,
    ITEM_HAND_SAW,
    ITEM_ADRENALINE,
    ITEM_BURNER_PHONE,
    ITEM_EXPIRED_MEDICATION,
    ITEM_INVERTER,
    ITEM_JAMMER,
    ITEM_REMOTE
} ItemType;

typedef struct {
    //
} MagnifyingGlass;

typedef struct {
    //
} Handcuffs;

typedef struct {
    //
} Beer;

typedef struct {
    //
} CigarettePack;

typedef struct {
    //
} HandSaw;

typedef struct {
    //
} Adrenaline;

typedef struct {
    //
} BurnerPhone;

typedef struct {
    //
} ExpiredMedication;

typedef struct {
    //
} Inverter;

typedef struct {
    //
} Jammer;

typedef struct {
    //
} Remote;

typedef struct {
    ItemType type;
     union {
        MagnifyingGlass magnifying_glass;
        Handcuffs handcuffs;
        Beer beer;
        CigarettePack cigarette_pack;
        HandSaw hand_saw;
        Adrenaline adrenaline;
        BurnerPhone burner_phone;
        ExpiredMedication expired_medication;
        Inverter inverter;
        Jammer jammer;
        Remote remote;
    };
} Item;






Item create_item(ItemType type);
Item create_random_item();
char * get_item_name(ItemType type);

#endif