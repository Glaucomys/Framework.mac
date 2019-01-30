|***
 *	CLR.inc
 *	Framework Cleric Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
 Sub _CLR
	/if (${DebugFramework}) /echo in CLR sub

	/if (${Spawn[${MainAssist}].Type.NotEqual[Corpse]} && ${Spawn[${MainAssist}].PctHPs} <= ${Math.Calc[${${MainAssistClass}Point}*.${tmpHealOverride}]}) {
		/if (${DebugFramework}) /echo Going to heal MA "${MainAssist}" "${MainAssistType}" ${Spawn[${MainAssist} ${MainAssistType}].PctHPs} ${Macro.CurLine}
		/call NewCast ${Spawn[=${MainAssist}].ID} "Heals"
	} else /if (${AutoRezOn} && !${Spawn[${MainAssist} ${MainAssistType}].ID} && ${Select[${MainAssistType},Mercenary,Pet]}==0) {
		/call CastWhat "${AutoRezWith}" ${Spawn[${MainAssist}'s corpse].ID}
	} else /if ((${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.NotEqual[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName}Point}*.${tmpHealOverride}]}) || (${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.Equal[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${PetPoint}*.${tmpHealOverride}]})) {
		/if (${DebugFramework}) /echo newSingleHeal "${Spawn[${WorstHurt[both,1,200,TRUE]}].CleanName}" "${Spawn[${WorstHurt[both,1,200,TRUE]}].Type}" ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs}
		/call NewCast ${Spawn[${WorstHurt[both,1,200,TRUE]}].ID} "Heals"
	} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
		/call NewCast ${MyTargetID} "DPS"
	}
	
	|/varset tmpHealOverride 99.99
	/if (${DebugHeal}) /echo \amDEBUGHEALS Class Sub leave \agLine#: ${Macro.CurLine}
	
	/if (${AutoRezOn}) /call RezCheck
	
/return 

 Sub _OLDCLR
	/if (${DebugFramework}) /echo in CLR sub
	/if (${Select[${Zone.ID},795,796]}!=0) {
		/varset tmpHealOverride 85.00
	}

	/if (${Spawn[${MainAssist}].Type.NotEqual[Corpse]} && ${Spawn[${MainAssist}].PctHPs} <= ${Math.Calc[${${MainAssistClass}Point}*.${tmpHealOverride}]}) {
		/if (${DebugFramework}) /echo Going to heal MA "${MainAssist}" "${MainAssistType}" ${Spawn[${MainAssist} ${MainAssistType}].PctHPs} ${Macro.CurLine}
		/call NewCast ${Spawn[=${MainAssist}].ID} "Heals"
	} else /if (${AutoRezOn} && !${Spawn[${MainAssist} ${MainAssistType}].ID} && ${Select[${MainAssistType},Mercenary,Pet]}==0) {
		/call CastWhat "${AutoRezWith}" ${Spawn[${MainAssist}'s corpse].ID}
	} else /if (${Select[${Me.Class.ShortName},BST,CLR,SHM,DRU,PAL]} && ${Group} && ${Group.Injured[${Math.Calc[${grpheal}*.${tmpHealOverride}]}]} > 1) {
		/if (${DebugFramework}) /echo Going to heal Group
		/call NewCast ${MyTargetID} "GHeals"
	} else /if ((${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.NotEqual[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName}Point}*.${tmpHealOverride}]}) || (${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.Equal[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${PetPoint}*.${tmpHealOverride}]})) {
		/if (${DebugFramework}) /echo newSingleHeal "${Spawn[${WorstHurt[both,1,200,TRUE]}].CleanName}" "${Spawn[${WorstHurt[both,1,200,TRUE]}].Type}" ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs}
		/call NewCast ${Spawn[${WorstHurt[both,1,200,TRUE]}].ID} "Heals"
	} else {
		|/echo In Clr sub attacks  
		/if (${CombatStart} && (${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} || ${Spawn[${MyTargetID}].ID} || !${Me.Feigning})) {
			/call NewCast ${MyTargetID} "DPS"
		}
	}
	
	|/varset tmpHealOverride 99.99
	/if (${DebugHeal}) /echo \amDEBUGHEALS Class Sub leave \agLine#: ${Macro.CurLine}
	
	/if (${AutoRezOn}) /call RezCheck
	
/return 