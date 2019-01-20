|***
 *	Manual.inc
 *	Framework Quest Subs Include
 *
 *	
 ***|

| ---------------------------------------------------------------------------- 
| SUB: Event_CAST_STUNNED
| ---------------------------------------------------------------------------- 
    Sub manual_SHD
	
        |/call SHD_checkbuffs
		
		/if (${Me.CombatState.Equal[COMBAT]} && ${Me.AltAbilityReady[Harm Touch]}) /Echo Harm Touch
		/if (${Me.CombatState.Equal[COMBAT]} && ${Me.AltAbilityReady[87]}) /Echo Leech Touch
		/if (${Me.CombatState.Equal[COMBAT]} && ${Me.AltAbilityReady[1451]}) /Echo Second Spire
		/if (${Me.CombatState.Equal[COMBAT]} && ${Me.AltAbilityReady[9403]}) /Echo Visage of Death
    /return
	Sub SHD_checkbuffs
	/if (!${Me.Buff[Drape of the Wrathforged].ID}) {
	/popup Rebuff Drape of the Wrathforged 	
	}
	/if (!${Me.Buff[Remorseless Demeanor].ID}) {
	/popup Rebuff Remorseless Demeanor 
	}
	/if (!${Me.Buff[Zombie Skin].ID}) {
	/popup Rebuff Zombie Skin
	}
	/if (!${Me.Buff[Shroud of the Doomscale].ID}) {
	/popup Rebuff Shroud of the Doomscale 
	}
	/if (!${Me.Buff[Vizat's Skin].ID}) {
	/popup Rebuff Vizat's Skin
	}
	/if (!${Me.Buff[Vizat's Horror].ID}) {
	/popup Rebuff Vizat's Horror
	}
	/if (!${Me.Buff[Voice of thule].ID}) {
	/popup Rebuff Voice of thule
	}
	/return
	
	Sub event_mortalcoil
	/echo MC proc
	/varset mctimer ${mclength}s
	/if (${startswarm}<1) {
		/varset startswarm 1
			}
	/return
	
Sub event_epic
	/if (${startswarm}<1) {
	/echo Epic clicked, starting swarm
	/varset startswarm 1
	}
	/return
	
Sub event_oorange
	/popup  <<<: YOUR TARGET IS OUT OF RANGE  :>>>
	/return
	
Sub event_cannotsee
	/popup <<<: YOU CANNOT SEE YOUR TARGET :>>>
	/return

Sub event_exp
	/echo Got a kill!
	/varcalc killcount ${killcount}+1
	/varcalc totalkills ${totalkills}+1
	/return

Sub event_expp
	/echo Got a kill!
	/varcalc killcount ${killcount}+1
	/varcalc totalkills ${totalkills}+1
	/return

Sub event_jumpstart
	/varset startswarm 1
	/return
	
Sub event_swarmover
	/if (${startswarm}>0) {
	/echo Ending the swarm by hand
	/varset forcestop 1
	}
	/return
	
Sub event_selftarget
	/keypress esc
	/return
	
Sub event_skintick
	/varcalc skincount ${skincount}+1
	/if (${skincount}>=35) {
	/popup Recast Skin
	/varset skincount 0
	}
	/return
	
Sub event_skinreset
	/varset skincount 0
	
	/return