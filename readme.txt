
    ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿   ÚÄÄÄÄ¿  ÚÄÄÄÄ¿   ÚÄÄÄÄ¿           ÚÄÄÄÄ¿  ÚÄÄÄÄ¿
    ³ ²²²²²²²²²² ³°  ³ ²² ³° ³ ²² ³°  ³ ²² ³°          ³ ²² ³° ³ ²² ³°
    ³ ²² ÚÄÄÄÄÄÄÄÙ°  ³ ²² ³° ³ ²² ³°  ³ ²² ³°          ³ ²² ³° ³ ²² ³°
    ³ ²² ³°°°°°°°°°  ³ ²² ÀÄÄÙ ²² ³°  ³ ²² ³°          ³ ²² ³° ³ ²² ³°
    ³ ²² ³°          ³ ²²²²²²²²²² ³°  ³ ²² ³°          ³ ²² ³° ³ ²² ³°
    ³ ²² ³°          ÀÄÄÄ¿ ²² ÚÄÄÄÙ°  ³ ²² ³°          ³ ²² ³° ³ ²² ³°
    ³ ²² ÀÄÄÄÄÄÄ¿        ³ ²² ³°°°°°  ³ ²² ÀÄÄÄÄÄÄ¿    ³ ²² ÀÄÄÙ ²² ³°
    ³ ²²²²²²²²² ³°       ³ ²² ³°      ³ ²²²²²²²²² ³°   ³ ²²²²²²²²²² ³°
    ÀÄÄÄÄÄÄÄÄÄÄÄÙ°       ÀÄÄÄÄÙ°      ÀÄÄÄÄÄÄÄÄÄÄÄÙ°   ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ°
     °°°°°°°°°°°°°        °°°°°°       °°°°°°°°°°°°°    °°°°°°°°°°°°°° v1.06

                Designed & programmed by Paul S.J.Millard (2001)

    <DISCLAIMER>

    This software is provided as is without any guarantees or
    warranty. Although I, the author, have attempted to find and correct
    any bugs in the software provided, I am not responsible for any
    damage or losses of any kind caused by the use or misuse of this
    software.  I, the author, are under no obligation to provide service,
    corrections, or upgrades to this package.

    <COPYRIGHT>

    This software and related documentation are (C) Paul S.J.Millard 2001.
    Although Cylu has been provided to you as FreeWare with no limitations
    applied to the game, donations of cash or cheque are greatly appreciated.
    Please see contact section for details if you feel like the giving mood :)

    <Distribution>

    Any on-line vendors or media packagers are welcome to distribute this
    software, although I will ask that you only distribute the software in
    it's original form without modification.  Any custom changes you feel
    are necessary to the package, written permission is required from the
    author.  Please see contact section for details.

    (Todo)

    1.  Currently I have not implemented sound effects or music

    2.  Proper cross platform compatibility.  I've compiled and tested
        under DOS and Linux but distributable versions will not be available
        for Linux until I've implemented sound.

        Any other suggestions, please email me.

ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

    CONTENTS.

    1.      Introduction

    2.      The story

    3.      Hardware Requirements
    3.1         Minimum Requirements
    3.2         Recommended Requirements

    4.      Commandline Options

    5.      How to play
    5.1         The main menu
    5.2         During the game
	5.3			Playing
	5.4			Special objects

    6.      Credits

    7.      Contact me.

ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

    1.  <Introduction>

    Welcome to Cylu.  I'd like to take this opportunity to thank you for
    acquiring this game and I hope it gives you hours of enjoyment as it
    has for me developing it.  Set the way back machine to 1985.  In the
    days of the humble Amstrad CPC computer, one of my fav games was Cylu.
    This game was originally created by Geoff Foley and published by
    Firebird Software.  Luck passed my way when I managed to obtain a map
    of the original game level scribbled on a piece of paper.  This gave
    me the inspiration to recreate the game with a touch of polish.

    .......And here we have it.

    2.  <The Story>

    Cylu the Otsan, the chosen one, enters the land of Vole to participate
    in the final challenge.  The aim is simple, the quest is long.
    The Otsan require a new leader, so a test has been devised which is
    presented here.  You will require wits, speed, cunning and logic to be
    worthy as the Otsan leader.  To complete the test, deposit all 24
    objects into the central computer.  This will require a knowledge
    of the planet and certain areas are only accessible via the teleportation
    network.

    Good luck.

    3.  <Hardware Requirements>

    3.1     Minimum Requirements

        Pentium 200Mhz Compatible PC
        Windows 98 & DirectX 7
        16MB Physical memory
        5Mb Free HD space
        4MB PCI VGA Video card (Vesa 1.0 +)

        I've tested the game on a machine with the above spec and
        although certain aspects of the game are slower than they
        should be, the game is configurable via the command line
        when starting the game.

    3.2     Recommended Requirements

        Pentium II MMX 400Mhz or faster Compatible PC
        32MB or more Physical memory
        10Mb Free HD Space
        8MB AGP VGA Video card (Vesa 2.0 +    Better performance )
        AWE32 or Sound Blaster compatible

        This was the machine I developed the game on.

    4.  <Commandline Options>

    Although the game runs happily on the minimum hardware requirements
    for a PC, you may find that you will want to give it that extra kick.
    Cylu can be configured in various ways to speed up the game during
    play by providing extra command line options when starting the game.

    As an example, just type at the DOS prompt:

        .\cylu -norotate

    Here is a summary of the options:

        -novsync        -   Turns off video synchronisation.  This will
                            variably speed up graphics updates.

        -norotate       -   On slower machines, this will prevent image
                            rotation calls from being executed.  This is
                            ideal for slow machines as this speeds up
                            the frame rate considerably, but doesn't look
                            as good ;)

        -nosound        -   Disables all music and digital effects playback
                            Use this option if you have trouble running the
                            game with your sound card.

    5.  <How to play>

    After you've copied all files to your hard disk, start the game by
    typing CYLU at the DOS prompt.

    5.1     <The main menu>

    The main menu will appear after pressing <ESC>

    Options are:    NEW GAME
                    HOW TO PLAY
                    CREDITS
                    EXIT

    Use the arrow keys to navigate the menu and press <ENTER> to select.
    I strongly recommend you look at "HOW TO PLAY" to see more details of
    how to play the game.

    5.2     <During the game>

    During play of the game, pressing <ESC> will bring up the game's sub
    menu.

    Options are:    CONTINUE
                    SAVE GAME
                    LOAD GAME
                    ABANDON

    Continue will remove the menu.  Pressing <ESC> has the same effect.

    Save Game will save the current state of the game so you may continue
    at a later time.

    Load Game will restore the last saved position of the game.

    End Game will finish the game and return you to the main menu.

	5.3		<Playing>

	After beginning a new game, you will start standing next to the central
	computer.  This is where you must deposit all 24 objects which are
	scattered throughout the planet.  Situated at the bottom left of the
	screen is your inventory.  You can carry a maximum of five objects at
	any one time.  To the right are your stats.  Your stats show how much
	fuel you have remaining, the number of objects deposited and the number
	of paces you've walked.

	Keys are:

		<ARROW KEYS>		Move in the selected direction

		<TAB>				Select one of your inventory pockets

		<SPACE>				Pick up an object you're facing

		<BACKSPACE>			Drop one of the objects from your inventory

		<ENTER>				Use the specified object

		<X>					Examine the selected object from your inventory
							If you're facing a forcefield, this action will
							describe what forcefield number you're facing

		<ESC>				Bring up the game sub menu

	To deposit items into the cental computer, face the computer and select
	the object from your inventory, then press <ENTER> for use.

	Scattered throughout the planet are forcefields.  These will prevent
	you from accessing certain areas of the planet.  Each forcefield has
	an associated CPU chip assigned to it.  To disable the relevant
	forcefield, pick up the chips and deposit them into the central computer
	By examining a chip, you will know which forcefield it disables.

	During play, you will also find teleporter pads and teleport keys.  Each
	key is assigned to a specific teleport pad placed somewhere on the planet
	You will need to use the teleportation system to gain access to areas
	of the planet.  To transport yourself, pick up a key and face one of
	the teleport pads.  By pressing <ENTER>, you will be transported to
	the designated teleport pad assigned by the key.

	HINT:	Try not to get yourself stuck.

	5.4		<Special Objects>

	There are two special objects you will come across during play.
	RFL and RTB.

	RFL (ReFueL at base) will allow you to maintain your life support systems
	if there are no fuel canisters available.  You can only refuel yourself
	at the central computer.

	RTB (Return To Base) will instantly transport you back to the central
	computer.

	Enjoy!

ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

    6.  <Credits>

		Original game concept by Geoff Foley

		Original game published in 1985 by Firebird Software for the
		Amstrad CPC series computer

		Programmer - Paul S.J.Millard
		Graphics - Paul S.J.Millard
        Music - Not implemented yet
        Sound FX - Not implemented yet
		Level design - Geoff Foley

	Special thanks goes to the following parties

		DJ Delorie - For DJGPP v2.03
			( An excellent C/C++ compiler )

        Shawn Hargreaves - For Allegro v3.9.38 WIP
			( Undoubtedly the best game library available )

		The POV-Ray Team - for Persistence of Vision(TM) v3.02 (DOS)

		Denis Olivier - For POVLab v4.00 SP4 Build 139
			( An excellent CAD package for POV-Ray )
                
        Robert Hohne - For RHIDE v1.4
			( Scrub those MAKE files ;) )

	And lastly but not least...
	Tanya, my loving girlfriend, for her support and them coffees (yum)

ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

	7.	<Contact me>

	Email:		apex@apexnow.co.uk

	WEB:		http://www.apexnow.co.uk

