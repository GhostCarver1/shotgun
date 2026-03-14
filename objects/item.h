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


/*
Magnifying Glass
----------------
Allows the player to inspect the next shell in the shotgun without firing it.

Gameplay effect:
• Reveal whether the next shell is LIVE or BLANK
• Does NOT consume the shell
• Useful for planning the next move
*/
typedef struct {
    // Could store last inspected shell index in the future
} MagnifyingGlass;


/*
Handcuffs
---------
Used on another player to skip their next turn.

Gameplay effect:
• Target player cannot act on their next turn
• Their turn is automatically skipped
*/
typedef struct {
    // Could store which player is currently restrained
} Handcuffs;


/*
Beer
----
Used to eject the next shell from the shotgun.

Gameplay effect:
• Remove the next shell from the chamber
• Reveals whether the removed shell was live or blank
• Useful for manipulating probability
*/
typedef struct {
    // No state needed currently
} Beer;


/*
Cigarette Pack
--------------
Restores health to the player.

Gameplay effect:
• Heal the player by 1 health
• Cannot exceed maximum health
*/
typedef struct {
    // Could store number of cigarettes remaining if needed
} CigarettePack;


/*
Hand Saw
--------
Increases damage of the next live shell fired.

Gameplay effect:
• Next LIVE shell deals double damage
• Effect disappears after one shot
*/
typedef struct {
    // Could store whether effect is currently active
} HandSaw;


/*
Adrenaline
----------
Allows the player to steal and immediately use an item from another player.

Gameplay effect:
• Choose another player
• Select one of their items
• Use it instantly
*/
typedef struct {
    // No state needed
} Adrenaline;


/*
Burner Phone
------------
Provides information about the future shells in the shotgun.

Gameplay effect:
• Reveals the position of a live shell somewhere ahead in the chamber
• Gives the player strategic information
*/
typedef struct {
    // Could store how many shells ahead were revealed
} BurnerPhone;


/*
Expired Medication
------------------
Risky healing item.

Gameplay effect:
• 50% chance to heal the player
• 50% chance to lose health instead
*/
typedef struct {
    // Could store RNG seed or effect result
} ExpiredMedication;


/*
Inverter
--------
Flips the type of the next shell.

Gameplay effect:
• LIVE becomes BLANK
• BLANK becomes LIVE
*/
typedef struct {
    // Could store whether the inversion effect is pending
} Inverter;


/*
Jammer
------
Disables another player's ability to use items temporarily.

Gameplay effect:
• Target player cannot use items for a certain number of turns
*/
typedef struct {
    // Could store remaining turns of item disable
} Jammer;


/*
Remote
------
Allows manipulation of the shotgun or turn order remotely.

Possible gameplay effects:
• Skip the next shell
• Trigger a chamber rotation
• Affect shotgun behavior from distance
*/
typedef struct {
    // Could store charges or remote state
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