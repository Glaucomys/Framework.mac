|***
 *	BER.inc
 *	Framework BERZERKER Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
 
 Sub _BER
	|/echo in class sub
        /if (${HealsOn} && ${Me.PctHPs} <= ${Math.Calc[${${Me.Class.ShortName}Point}*.${tmpHealOverride}]}) {
			/call newSingleHeal "${Me}" "PC" ${Me.PctHPs} 0 G
		} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
			|/call CombatCast
			|/call newcombatcast
			/call NewCast ${MyTargetID} "DPS"
		}

/return 

 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
 sub script_ber
	/if (${DebugFramework}) /echo in ber discs
	/if (${Me.CombatAbilityReady[${Spell[Disconcerting Discipline].RankName}]}) {
		/if (${DebugFramework}) /echo 1
		/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Disconcerting Discipline].RankName}]}].ID}
		
	} else /if (${Me.ItemReady[${epicaxe}]}) {
		/if (${DebugFramework}) /echo 2
		/disc ${Me.CombatAbility[${Me.CombatAbility[${epicaxe}]}].ID}
	} else /if (${Me.AltAbilityReady[Focused Furious Rampage]}) {
		/if (${DebugFramework}) /echo 3
		/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Brutal Discipline].RankName}]}].ID}
		/alt act 379
		/alt act 465 
		
		/alt act 3710 
		/alt act 961 
		/alt act 7019 
	} else /if (${Me.CombatAbilityReady[${Spell[Cleaving Acrimony Discipline].RankName}]}) {
		/if (${DebugFramework}) /echo 4
		/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Cleaving Acrimony Discipline].RankName}]}].ID}
		/alt act 1501
		/alt act 610
		/useitem "Cohort's Warmonger Coat"
	} else /if (${Me.CombatAbilityReady[${Spell[Frenzied Resolve Discipline].RankName}]}) {
		/if (${DebugFramework}) /echo 5
		/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Frenzied Resolve Discipline].RankName}]}].ID}
	} else /if (${Me.CombatAbilityReady[${Spell[Avenging Flurry Discipline].RankName}]}) {
		/if (${DebugFramework}) /echo 6
		/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Avenging Flurry Discipline].RankName}]}].ID}
	} 
/return