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
	/if (${DebugFramework}) /echo in class sub 
	/if (${HealsOn} && ${Me.PctHPs} <= ${Math.Calc[${${Me.Class.ShortName}Point}*.${tmpHealOverride}]}) {
		/call NewCast ${Me.ID} "Heals"
	} else /if (${DPSOn} && ${CombatStart} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && !${Me.Feigning}) {
		/call NewCast ${MyTargetID} "DPS"
	}

/return 

	sub script_ber
		/if (${DebugFramework}) /echo in ber discs
		/if (${Me.CombatAbilityReady[${Spell[Disconcerting Discipline].RankName}]}) {
			/if (${DebugFramework}) /echo 1
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Disconcerting Discipline].RankName}]}].ID}
		} else /if (${Me.ItemReady[${epicaxe}]}) {
			/if (${DebugFramework}) /echo 2
			/useitem "${epicaxe}"
			
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
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
	sub script_TBL_ber
		/if (${DebugFramework}) /echo in TBL ber discs
		/if (${Me.AltAbilityReady[Focused Furious Rampage]}) {
			/if (${DebugFramework}) /echo 3
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Brutal Discipline].RankName}]}].ID}
			|Savage
			/alt act 465
			|Juggernaut Surge
			/alt act 961
			|BloodPact
			/alt act 387
			|Blinding Fury
			/alt act 610
			|Silent Strikes
			/alt act 1109
			|Second Spire
			/alt act 1501
			|Desperation
			/alt act 373
			|Focused Rampage
			/alt act 379
			|Untamed Rage
			/alt act 374
			/useitem "${epicaxe}"
			/delay 15 !${Me.Casting.ID}
			/useitem "Cohort's Warmonger Coat"
			/delay 15 !${Me.Casting.ID}
		} else /if (${Me.CombatAbilityReady[${Spell[Cleaving Acrimony Discipline].RankName}]}) {
			/if (${DebugFramework}) /echo 4
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Cleaving Acrimony Discipline].RankName}]}].ID}
			|Reckless Abandon
			/alt act 3710
			|Vehement Rage
			/alt act 800
		} else /if (${Me.CombatAbilityReady[${Spell[Frenzied Resolve Discipline].RankName}]}) {
			/if (${DebugFramework}) /echo 5
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Frenzied Resolve Discipline].RankName}]}].ID}
		} else /if (${Me.CombatAbilityReady[${Spell[Avenging Flurry Discipline].RankName}]}) {
			/if (${DebugFramework}) /echo 6
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Avenging Flurry Discipline].RankName}]}].ID}
		} else /if (${Me.AltAbilityReady[Reckless Abandon]}) {
			/alt act 3710
			/alt act 800
		} else /if (${Me.CombatAbilityReady[${Spell[Disconcerting Discipline].RankName}]}) {
			/if (${DebugFramework}) /echo 1
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Disconcerting Discipline].RankName}]}].ID}
		} else /if (${Me.ItemReady[${epicaxe}]}) {
			/if (${DebugFramework}) /echo 2
			/useitem "${epicaxe}"
		}
	/return