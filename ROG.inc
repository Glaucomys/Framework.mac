|***
 *	ROG.inc
 *	Framework Rogue Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
Sub _ROG
	/if (${DebugFramework}) /echo in class sub
	/call Roguestuff
	/if (${HealsOn} && ${Me.PctHPs} <= ${Math.Calc[${${Me.Class.ShortName}Point}*.${tmpHealOverride}]}) {
		/call NewCast ${Me.ID} "Heals"
	} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
		/call NewCast ${MyTargetID} "DPS"
	}
/return 
| ----------------------------------------------------------------------------
| SUB: Roguestuff
| ----------------------------------------------------------------------------
    Sub Roguestuff
        /if (${Me.Hovering}) /return
        /if (${Debug}) /echo \atDEBUGBUFFS Roguestuff Enter \agLine#: ${Macro.CurLine}
        /if (${Me.Class.Name.Equal[Rogue]} && !${Me.Combat} && ${AutoHide}) {
            /if (${Me.AbilityReady[hide]} && ${Me.AbilityReady[sneak]}) {
                /doability hide
                /doability sneak
            }
        }
    /if (${Debug}) /echo \atDEBUGBUFFS Roguestuff Leave \agLine#: ${Macro.CurLine}
    /return
