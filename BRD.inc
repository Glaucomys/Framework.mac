|***
 *	BRD.inc
 *	Framework Bard Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
 Sub _BRD
	/if (${DebugFramework}) /echo in class sub 
	/if (${MezOn} && ${Me.XTarget}>1 && ${SpawnCount[NPC radius 30]}>1 &&${Me.GemTimer[${MezAESpell}]}==0 && ${Me.CurrentMana}>${Spell[${MezAESpell}].Mana} && ${Me.Gem[${MezAESpell}]}) {
		/squelch /twist once ${Me.Gem[${MezAESpell}]}
		/call BroadCast ${EQBCOn} g "MEZZING-> AE Mezzing Now - ${MezAESpell} "
	} else /if (${MezOn}&& ${Me.XTarget}>1 && ${SpawnCount[NPC radius 30]}>1) {
			/if (${DebugFramework}) /echo checking Mez
			/varset MezBroke 0
			/doevents MezBroke
			/if (!${MezBroke}) /call DoMezStuff
	}
	/if (!${CombatStart} && ${IAmABard}) /call DoBardStuff
	/if (${HealsOn} && ${Me.PctHPs} <= ${Math.Calc[${${Me.Class.ShortName}Point}*.${tmpHealOverride}]}) {
		/call NewCast ${Me.ID} "Heals"
	} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
		/call NewCast ${MyTargetID} "DPS"
	}

	
/return