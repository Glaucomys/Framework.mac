|***
 *	PAL.inc
 *	Framework Paladin Include
 *
 *	
 ***|
 
 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
Sub _pal
	/if (${DebugFramework}) /echo in class sub
	/if (${Select[${Role},tank,pettank,pullertank,pullerpettank,hunter,hunterpettank]} && ${ReturnToCamp} && ${Math.Distance[${CampYLoc},${CampXLoc}]}>20) {
	    
		/varset ReturnToCamp 1
		/call DoWeMove 1 CheckForCombat 
		/if (${MyTargetID}) {
			/if (${Target.ID}!=${MyTargetID} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].ID} && ${DPSOn} && ${MeleeOn}) {
				/squelch /target id ${MyTargetID}
				/delay 10 ${Target.ID}==${MyTargetID}
			}
		}
		/if (${Target.ID}) /face ${If[${FaceMobOn}==2,nolook,fast nolook]}
	}
	/if (${Me.CombatState.Equal[COMBAT]} && ${Select[${Me.Class.ShortName},WAR,PAL,SHD]} && ${CombatStart} && !${Me.ActiveDisc.ID} && (${Target.Named} || ${Me.XTarget}>3) && ${Target.Distance}<75) {
		/if (${DebugFramework}) /echo defensive
		/call PalDefense
	} else /if (${Spawn[${MainAssist}].Type.NotEqual[Corpse]} && ${Spawn[${MainAssist}].PctHPs} <= ${Math.Calc[${${MainAssistClass}Point}*.${tmpHealOverride}]}) {
		/if (${DebugFramework}) /echo ${Macro.CurLine}
		/call NewCast ${Spawn[=${MainAssist}].ID} "Heals"
	} else /if (${AutoRezOn} && !${Spawn[${MainAssist} ${MainAssistType}].ID} && ${Select[${MainAssistType},Mercenary,Pet]}==0) {
		/call CastWhat "${AutoRezWith}" ${Spawn[${MainAssist}'s corpse].ID}
	} else /if ((${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.NotEqual[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName}Point}*.${tmpHealOverride}]}) || (${Spawn[${WorstHurt[both,1,200,TRUE]}].Type.Equal[Pet]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].Class.ShortName.NotEqual[Corpse]} && ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} <= ${Math.Calc[${PetPoint}*.${tmpHealOverride}]})) {
		/if (${DebugFramework}) /echo newSingleHeal "${Spawn[${WorstHurt[both,1,200,TRUE]}].CleanName}" "${Spawn[${WorstHurt[both,1,200,TRUE]}].Type}" ${Spawn[${WorstHurt[both,1,200,TRUE]}].PctHPs} 0 G
		/call NewCast ${Spawn[${WorstHurt[both,1,200,TRUE]}].ID} "Heals"
	}  else /if (${SpawnSort[1,asc,PctAggro,xtarhater].PctAggro} < 100 && ${CombatStart} && ${Select[${Role},pullertank,tank]}) {
		/if (${DebugFramework}) /echo aggro
		/call NewCast ${SpawnSort[1,asc,PctAggro,xtarhater].ID} "Aggro"
	} else /if (${DPSOn} && ${CombatStart} && (${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} || ${Spawn[${MyTargetID}].ID} || !${Me.Feigning})) {
		/call NewCast ${MyTargetID} "DPS"
	}
	/if (${AutoRezOn} && ${SpawnCount[PC corpse radius 100]} > 0) /call RezCheck
/return 


 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|

Sub PalDefense
	/if (!${Me.ActiveDisc.ID}) {
		/if (${Me.AltAbilityReady[Group Armor of the Inquisitor]}) {
			/call CastWhat "Group Armor of the Inquisitor"
			/if (${Macro.Return.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Group Armor of the Inquisiton" r "Active"
			/return				
			}
		/if (${Me.PctEndurance}>5) {
			/if (${Me.CombatAbilityReady[${Spell[Holy Guardian Discipline].RankName}]}) {
				/if (${Spell[${Spell[Blessed Guardian Discipline].RankName}].ID}) {
					/disc ${Spell[${Spell[Blessed Guardian Discipline].RankName}].ID}
					} Else {
					/disc ${Spell[${Spell[Holy Guardian Discipline].RankName}].ID}
					}
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Guardian Discipline, Defense line 1" r "Active"
				/return				
				}
			/if (!${Me.CombatAbilityReady[${Spell[Holy Guardian Discipline].RankName}]} && ${Me.CombatAbilityReady[${Spell[Honorific Mantle].RankName}]}) {
				/if (${Spell[${Spell[Kar'Zok Mantle].RankName}].ID}) {
					/disc ${Spell[Kar'Zok Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Skalber Mantle].RankName}].ID}) {
					/disc ${Spell[Skalber Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Brightwing Mantle].RankName}].ID}) {
					/disc ${Spell[Brightwing Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Prominent Mantle].RankName}].ID}) {
					/disc ${Spell[Prominent Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Honorific Mantle].RankName}].ID}) {
					/disc ${Spell[Honorific Mantle].RankName}
					/goto :PALMantleEnd
					}
				/if (${Spell[${Spell[Armor of Endless Honor].RankName}].ID}) {
					/disc ${Spell[Armor of Endless Honor].RankName}
					/goto :PALMantleEnd
					}					
				:PALMantleEnd
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Mantle Discipline, Defense line 2" r "Active"	
				/return
				}
			/if (!${Me.CombatAbilityReady[${Spell[Honorific Mantle].RankName}]} && ${Me.CombatAbilityReady[${Spell[Armor of Courage].RankName}]}) {	
				/if (${Spell[${Spell[Armor of Mercy].RankName}].ID}) {
					/disc ${Spell[Armor of Mercy].RankName}
					/goto :PALArmorEnd
					}		
				/if (${Spell[${Spell[Devout Penitence].RankName}].ID}) {
					/disc ${Spell[Devout Penitence].RankName}
					/goto :PALArmorEnd
					}
				/if (${Spell[${Spell[Reverent Penitence].RankName}].ID}) {
					/disc ${Spell[Reverent Penitence].RankName}
					/goto :PALArmorEnd
					}
				/if (${Spell[${Spell[Armor of Zeal].RankName}].ID}) {
					/disc ${Spell[Armor of Zeal].RankName}
					/goto :PALArmorEnd
					}
				/if (${Spell[${Spell[Armor of Courage].RankName}].ID}) {
					/disc ${Spell[Armor of Courage].RankName}
					/goto :PALArmorEnd
					}		
				:PALArmorEnd
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Penitence Discipline, Defense line 3" r "Active"
				/return 		
				}
			}
		/if (!${Me.CombatAbilityReady[${Spell[Armor of Courage].RankName}]}) {
			/if (${Me.AltAbilityReady[Armor of Experience]}) {
				/call CastWhat "Armor of Experience"
				/if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience" r "Active"
				/return				
				}
			/if (${Me.AltAbilityReady[Fundament: Third Spire of Holiness]}) {
				/call CastWhat "Fundament: Third Spire of Holiness"
				/if (${Macro.Return.Equal[CAST_SUCCESS]} && ${ReportDefense}) /call AnnounceMessage 2 y "First Spire" r "Active"
				/return			
				}
			}
	}
/return