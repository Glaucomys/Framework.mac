|***
 *	NEC.inc
 *	Framework NECROMANCER Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
Sub _nec
    /if (${DebugFramework}) /echo in class sub
    /if (${Spawn[${MyTargetID}].PctHPs}<=${PetAssistAt} && ${PetOn} && ${PetCombatOn} && !${Me.Pet.Combat}) {
		/call CombatPet
	}
	/if (${HealsOn} && ${Me.PctHPs} <= ${Math.Calc[${${Me.Class.ShortName}Point}*.${tmpHealOverride}]}) {
		/call NewCast "${Me}" "Heals"
	} else /if ((${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.Equal[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${PetPoint}*.${tmpHealOverride}]})) {
		/call NewCast ${Spawn[${WorstHurt[both,1,200,TRUE]}].ID} "Heals"
	} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
		/if (${DebugFramework}) /echo in combat
		/call NewCast ${MyTargetID} "DPS"
	}
/return