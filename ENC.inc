|***
 *	ENC.inc
 *	Framework ENCHANTER Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
Sub _enc
	/if (${DebugFramework}) /echo in class sub
    /if (${MezOn}) {
		/if (${DebugFramework}) /echo checking Mez
		/varset MezBroke 0
		/doevents MezBroke
		/if (!${MezBroke}) /call DoMezStuff
	}
	
	/if (${PetOn} && ${PetCombatOn} && !${Me.Pet.Combat} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].PctHPs}<=${PetAssistAt} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].Distance}<${MeleeDistance}) {
		/if (${DebugFramework}) /echo In Combatpet
		/call CombatPet
	}
	
	/if (${HealsOn} && ${Me.PctHPs} <= ${Math.Calc[${${Me.Class.ShortName}Point}*.${tmpHealOverride}]}) {
		/if (${DebugFramework}) /echo in Heals
		/call NewCast ${Me.ID} "Heals"
	} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
		/if (${DebugFramework}) /echo in DPS
		/call NewCast ${MyTargetID} "DPS"
	}

/return 

