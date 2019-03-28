|
|	SHD.inc
|	Framework Shadow Knight Include

Sub _shd
	/if (${Debug}) /echo _SK Enter Line#: ${Macro.CurLine}
	/if (${Select[${Role},tank,pettank,pullertank,pullerpettank,hunter,hunterpettank]} && ${ReturnToCamp} && ${Math.Distance[${CampYLoc},${CampXLoc}]}>20) {
	    /varset ReturnToCamp 1
		/call DoWeMove 1 CheckForCombat
	}
	
	/if (${Me.CombatState.Equal[COMBAT]} && ${Select[${Me.Class.ShortName},WAR,PAL,SHD]} && ${CombatStart} && !${Me.ActiveDisc.ID} && (${Target.Named} || ${Me.XTarget}>1) && ${Target.Distance}<75) {
		/if (${DebugFramework}) /echo defensive
		/call SKDefense
	} 
	
	/if (${SpawnSort[1,asc,PctAggro,xtarhater].PctAggro} < 100 && ${CombatStart} && ${Select[${Role},pullertank,tank]}) {
		
		/echo controlling aggro on ${SpawnSort[1,asc,PctAggro,xtarhater].CleanName} 
		/call NewCast ${SpawnSort[1,asc,PctAggro,xtarhater].ID} "Aggro"
	} else /if (${DPSOn} && ${CombatStart} && (${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} || ${Spawn[${MyTargetID}].ID} || !${Me.Feigning})) {
		/call NewCast ${MyTargetID} "DPS"
	}
	
	/if (${CombatStart} && ${PetOn} && ${PetCombatOn} && !${Me.Pet.Combat} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].PctHPs}<=${PetAssistAt} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].Distance}<${MeleeDistance}) {
		/call CombatPet
	}
	
/return 


Sub SKDefense
	/if (${DebugFramework}) /echo in skdefense
	/if (${FindItem[=Diplomatic Papers].ID} && ${Me.ItemReady[Diplomatic Papers]} && (${Me.XTarget} > 3 || ${Target.Named})) /call CastWhat "Innoruuk's Dark Blessing"
	/if (!${Me.ActiveDisc.ID} && !${Me.Buff[Armor of Experience].ID} && !${Me.Buff[Glyph of Dragon Scales].ID} && !${Me.Song[Lich Sting Recourse].ID}) {
	
		/if (${Me.CombatAbilityReady[Deflection Discipline]} && (${Me.XTarget} > 3 || ${Target.Named}) && !${Me.CombatAbilityReady[${Spell[Unholy Guardian Discipline].RankName}]} && !${Me.CombatAbilityReady[${Spell[Doomscale Mantle].RankName}]} && !${Me.CombatAbilityReady[${Spell[Vizat's Carapace].RankName}]} && !${Me.CombatAbilityReady[${Spell[Unholy Aura Discipline].RankName}]}) {
			/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Deflection Discipline].RankName}]}].ID}
		}
		
		/if (${DebugFramework}) /echo past deflect
		/if ((${Me.XTarget} > 3 || ${Target.Named} || ${Me.PctHPs} < 20) && !${Me.CombatAbilityReady[Deflection Discipline]} && !${Me.AltAbilityReady[Shield Flash]}) {
			/call CastWhat "Shield Flash"
		}
		/if (${DebugFramework}) /echo past shield flash
		/if (${Me.PctEndurance}>5) {
			/if (${Me.CombatAbilityReady[${Spell[Unholy Guardian Discipline].RankName}]}) {
				/if (${Me.CombatAbilityReady[${Spell[Cursed Guardian Discipline].RankName}]}) {
					/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Cursed Guardian Discipline].RankName}]}].ID}
				} else {
					/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Unholy Guardian Discipline].RankName}]}].ID}
				}
			
				/return				
			}
			
			/if (${DebugFramework}) /echo past gaurdian
			/if (${Me.CombatAbilityReady[${Spell[Unholy Aura Discipline].RankName}]}) {
				/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Unholy Aura Discipline].RankName}]}].ID}
				
				/return				
			}
			
			/if (${DebugFramework}) /echo past Aura
			
			/if (!${Me.CombatAbilityReady[${Spell[Unholy Guardian Discipline].RankName}]} && ${Me.CombatAbilityReady[${Spell[Ichor Guard].RankName}]}) {
			
				/if (${Me.CombatAbilityReady[${Spell[Krellnakor Mantle].RankName}]}) {
					/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Krellnakor Mantle].RankName}]}].ID}
					/goto :SKMantleEnd
				}
				
				/if (${Me.CombatAbilityReady[${Spell[Doomscale Mantle].RankName}]}) {
					/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Doomscale Mantle].RankName}]}].ID}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Bonebrood Mantle].RankName}].ID}) {
					/disc ${Spell[Bonebrood Mantle].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Gorgon Mantle].RankName}].ID}) {
					/disc ${Spell[Gorgon Mantle].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Malarian Mantle].RankName}].ID}) {
					/disc ${Spell[Malarian Mantle].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Umbral Carapace].RankName}].ID}) {
					/disc ${Spell[Umbral Carapace].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Soul Carapace].RankName}].ID}) {
					/disc ${Spell[Soul Carapace].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Soul Shield].RankName}].ID}) {
					/disc ${Spell[Soul Shield].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Soul Guard].RankName}].ID}) {
					/disc ${Spell[Soul Guard].RankName}
					/goto :SKMantleEnd
				}
				/if (${Spell[${Spell[Ichor Guard].RankName}].ID}) {
					/disc ${Spell[Ichor Guard].RankName}
					/goto :SKMantleEnd
				}					
				:SKMantleEnd
				
				/return
			}
			/if (${DebugFramework}) /echo past mantle
			
			/if (!${Me.CombatAbilityReady[${Spell[Unholy Guardian Discipline].RankName}]} && !${Me.CombatAbilityReady[${Spell[Ichor Guard].RankName}]} && ${Me.CombatAbilityReady[${Spell[Grelleth's Carapace].RankName}]}) {	
				/if (${Me.CombatAbilityReady[${Spell[Tylix's Carapace].RankName}]}) {
					/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Tylix's Carapace].RankName}]}].ID}
					/goto :SKArmorEnd
				}
				/if (${Me.CombatAbilityReady[${Spell[Vizat's Carapace].RankName}]}) {
					/disc ${Me.CombatAbility[${Me.CombatAbility[${Spell[Vizat's Carapace].RankName}]}].ID}
					/goto :SKArmorEnd
				}
				/if (${Spell[${Spell[Grelleth's Carapace].RankName}].ID}) {
					/disc ${Spell[Grelleth's Carapace].RankName}
					/goto :SKArmorEnd
				}
				/if (${Spell[${Spell[Sholothian Carapace].RankName}].ID}) {
					/disc ${Spell[Sholothian Carapace].RankName}
					/goto :SKArmorEnd
				}
				/if (${Spell[${Spell[Gorgon Carapace].RankName}].ID}) {
					/disc ${Spell[Gorgon Carapace].RankName}
					/goto :SKArmorEnd
				}					
				:SKArmorEnd
				
				/return				
			}
		
			/if (${DebugFramework}) /echo past carapace
			
			
		}
		/if (!${Me.CombatAbilityReady[${Spell[Gorgon Mantle].RankName}]}) {
			/if (${Me.AltAbilityReady[Fundament: Third Spire of the Reaver]}) {
				/call CastWhat "Fundament: Third Spire of the Reaver"
				|/if (${Macro.Return.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Third Spire" r "Active"
				/return			
				}
			/if (!${Me.AltAbilityReady[Fundament: Third Spire of the Reaver]} && ${Me.AltAbilityReady[Armor of Experience]}) {
				/call CastWhat "Armor of Experience"
				|/if (${Macro.Return.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience"  r "Active"
				/return			
			}
			/if (!${Me.AltAbilityReady[Fundament: Third Spire of the Reaver]} && !${Me.AltAbilityReady[Armor of Experience]} && ${Me.AltAbilityReady[Visage of Death]}) {
				/call CastWhat "Visage of Death"
				/if (${FindItem[=Innoruuk's Dark Blessing].ID}) /call CastWhat "Innoruuk's Dark Blessing"
				|/if (${Macro.Return.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience"  r "Active"
				/return			
			}
			/if (${Me.CombatAbilityReady[${Spell[Leechcurse Discipline].RankName}]} && !${Me.AltAbilityReady[Fundament: Third Spire of the Reaver]} && !${Me.AltAbilityReady[Armor of Experience]} && ${Me.AltAbilityReady[Visage of Death]}) {
				/disc ${Spell[Leechcurse Discipline].RankName}
				|/if (${Macro.Return.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience"  r "Active"
				/return			
			}
			/if (!${Me.AltAbilityReady[Fundament: Third Spire of the Reaver]} && !${Me.AltAbilityReady[Armor of Experience]} && !${Me.AltAbilityReady[Visage of Death]} && ${Me.AltAbilityReady[Glyph of Dragon Scales]}) {
				/call CastWhat "Glyph of Dragon Scales"
				/if (${Me.AAPoints} > 40) /alt buy ${AltAbility[Glyph of Dragon Scales].Index}
				|/if (${Macro.Return.Equal[CAST_SUCCESS]}) /if (${ReportDefense}) /call AnnounceMessage 2 y "Armor of Experience"  r "Active"
				/return			
			}
		}
		
	}
/return

|Impenitent Influence is the first disc to use for large pulls, Krellnakor Mantle is the second, and Cursed Guardian is the 3rd
|Glyph of the Dragon Scales lasts 5 minutes and is a bit redundant with some discs
|Aligning Visage of Death (increased melee dps) with Epic Click is also a really good tool

 	|/if (${combat_function[]} && ${CombatStart}) {
	|		/echo combat_adj
	|		/call combat_adj
	|}
	