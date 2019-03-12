
	Sub Custom_declares
	/declare mctimerseconds int outer 0
	/declare mctimer timer outer 0
	/declare skincount int outer 0
	/declare cyclecount             int         outer       0
	/declare healcount             int         outer       0
	/declare SpawnSearchType	string outer npc
	/call LoadIni Mod_Heal selfheal									int	80	
	/call LoadIni Mod_Heal UseModHeal								int	 1
	/declare tmpHealOverride										float outer 99.99
	/declare badaggroid 											int outer 0
	/declare Mod_MostHurtName       string      outer		NULL
	/declare Mod_MostHurtType       string      outer		NULL
	/declare Mod_MostHurtID         int         outer       0
	/declare Mod_MostHurtHP         int         outer       100
	/declare Mod_MostHurtNo         int         outer       0
	/declare Mod_WorstClass         string      outer
	/declare Mod_GroupHealthAvg     int      	outer       0
	/declare Mod_HurtMember     	int 		outer 		0
	/declare NULLPoint     	int 		outer 		100
	/declare Tank1	 string outer Gegolian
	/declare Tank2	 string outer Oamuk
	/declare Tank3	 string outer Oamuk
	/declare Tank4	 string outer Oamuk
	/declare Tank5	 string outer Oamuk
	/declare Innymovevariable	 string outer Empty
	/declare ClericSPLASHROTATION int outer 0
	/declare Cleric1 string outer Amriel
	/declare Cleric2 string outer Suede
	/declare Cleric3 string outer Dennwiny
	/declare raidbuffup string outer NULL
	/declare movetotarget int outer 0
	/declare movecall int outer 0
	/declare AEOK int outer 0
	/declare CampXLoc2               int        outer       ${Me.X}
	/declare CampYLoc2              int         outer       ${Me.Y}
	/declare InnyXLoc               int         outer       ${Me.X}
	/declare InnyYLoc               int         outer       ${Me.Y}
	/declare ReportDefense bool outer FALSE

	/if (${Select[${Me.Class.ShortName},BER]}) {
		/declare epicaxe					string outer NULL
		/declare UseEpic					int outer 0
		/if (${FindItem[Raging Taelosian Alloy Axe].ID}) {
			/varset epicaxe		Raging Taelosian Alloy Axe
			/varset UseEpic 1
		} else /if (${FindItem[Vengeful Taelosian Blood Axe].ID}) {
			/varset epicaxe		Vengeful Taelosian Blood Axe
			/varset UseEpic 1
		}
	}
	/return

|----------------------------------------------------------------------------
|- SUB: Death Sub
|----------------------------------------------------------------------------
	
	Sub DeathSub
		/echo You are sleeping with the fishes
		/xtarget set 1 autohater
		:DeathSubwait
		/if (${Window[RespawnWnd].Open} && ${ClickBacktoCamp}==1) {
			/nomodkey /notify RespawnWnd RW_OptionsList listselect 1 
			/delay 1s
			/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
			/delay 5s ${Me.Zoning}
		}
		/if (${Me.Hovering}) /goto :DeathSubwait
		/delay 1m !${Me.Zoning}
		/if (${ClickBacktoCamp}==1 && ${FindItem["Fellowship Registration Insignia"].Timer}==0) {
			/delay 30s ${Me.CombatState.Equal[ACTIVE]}
			/useitem "Fellowship Registration Insignia"
			/delay 2s ${FindItem["Fellowship Registration Insignia"].Timer}!=0
		} else /if (${ClickBacktoCamp}==1 && ${FindItem["Fellowship Registration Insignia"].Timer}!=0) {
			/echo \aw Bummer, Insignia on cooldown, you must really suck at this game...
		} else {
			/echo \aw Waiting on someone that knows how to heal to \ar rez me
		}
		/if (${IAmDead} && ${CampZone}==${Zone.ID} && (${Me.Buff[Resurrection Sickness].ID} || ${SpawnCount[pccorpse ${Me}]}==0)) /varset IAmDead 0
	/return

|----------------------------------------------------------------------------
|- SUB: FindTarget - melee attack portion removed. Melee attack moved into engage sub to reduce too far away and can not see messages
|---------------------------------------------------------------------------- 
	
	Sub NewCast(CastTarID, Array, spellcast)
		/if (${DebugCombat}) /echo \atDEBUG NewCast Enter. ID: ${CastTarID}, Array: ${Array}, Spell: ${spellcast} \agLine#: ${Macro.CurLine}
		/doevents 
		
		/declare i int local
		/declare SkipFlag    int      local 0
		/varset CastResult CAST_NO_RESULT
		/declare CastThis string local
		/declare MashThisArg3 string local
		/declare MashThisArg4 string local
		/declare MaxTryNum int local 2
		/declare TryNum int local 0
		/declare CondNo      int      local 0
		/declare TarID      int      local ${CastTarID}
		/declare WaitTimerCD2 timer local 
		
		/for i 1 to ${${Array}.Size}
		
			/if (${Bool[${spellcast}]}) {
				/varset CastThis ${spellcast}
			} else {
				/varset CastThis ${${Array}[${i}].Arg[1,|]}
			}
			/while (1) {
				|/if (${${Array}[${i}].Find[|ambush]} || !${Bool[${${Array}[${i}].Arg[1,|]}]}) /break
				
				/if ((${Spawn[${TarID}].Type.NotEqual[pet]} && ${${Array}[i].Arg[3,|].Equal[pet]}) || (${Bool[${${Array}[${i}].Arg[3,|]}]} && (${${Array}[${i}].Arg[3,|].Find[mob]} || ${${Array}[${i}].Arg[3,|].Find[tap]}) && !${CombatStart})) /break
				/if (${Spawn[${TarID}].Type.Equal[Corpse]} || !${Spawn[${TarID}].ID}) {
					/if (${DebugCombat}) /echo CombatCast: Return from CombatCast cause my target is a corpse or dead or DPSPaused ${DPSPaused}. \agLine#: ${Macro.CurLine}
					/return
				}
				/if (${ConOn} && ${${Array}[${i}].Find[|cond]}) {
					/varset CondNo ${${Array}[${i}].Mid[${Math.Calc[${${Array}[${i}].Find[|cond]}+5]},3]}
				} else {
					/varset CondNo 0
				}
				/if ((${CondNo}==0 || (${CondNo} && ${If[${Cond[${CondNo}]},1,0]})) && ${CastCheck[${CastThis}]}) {
					/if (${Bool[${${Array}[${i}].Arg[3,|]}]} && (${${Array}[${i}].Arg[3,|].Find[mob]} || ${${Array}[${i}].Arg[3,|].Find[tap]}) && ${DPSOn} && ${Spawn[${MyTargetID}].Type.NotEqual[Corpse]} && ${Spawn[${MyTargetID}].ID} && ${CombatStart}) {
						/if (${DebugFramework}) /echo Casting ${CastThis} on Target ${Spawn[${MyTargetID}].CleanName} 
						/varset TarID ${MyTargetID}
					} else /if (${Bool[${${Array}[${i}].Arg[3,|]}]} && ${${Array}[${i}].Arg[3,|].Find[MA]}) {
						/if (${DebugFramework}) /echo Casting ${CastThis} on Target ${Spawn[${Spawn[=${MainAssist}].ID}].CleanName}
						/varset TarID ${Spawn[=${MainAssist}].ID}
					} else {
						/if (${DebugFramework}) /echo Casting ${CastThis} on Target ${Spawn[${CastTarID}].CleanName} 
						/varset TarID ${CastTarID}
					}
					/if (${Target.ID}!=${TarID} && ${Spawn[id ${TarID}].ID}) /call CastTarget ${TarID}
					|/echo ${CastThis}
					/if (${Me.Moving} && ${Spell[${CastThis}].CastTime}) /call StopMoving
					/if (${CastThis.Find[command:]}) {
						/if (${Bool[${${Array}[${i}].Arg[4,|]}]}) {
							/docommand ${${Array}[${i}].Arg[3,|]} ${${Array}[${i}].Arg[4,|]}
						} else {
							/docommand ${${Array}[${i}].Arg[3,|]}
						}
						/if (${DebugFramework}) /echo ${CastThis} Command ${TarID} from NewCast
						/if (${EQBCOn}) /call BroadCast ${EQBCOn} r "Command >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
					} else /if (${CastThis.Find[script]}) {
						/call script_${Me.Class.ShortName}
						/if (${DebugFramework}) /echo ${CastThis} Script ${TarID} from NewCast
						/if (${EQBCOn}) /call BroadCast ${EQBCOn} r "Script >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
					} else /if (${FindItem[=${CastThis}].ID} && ${Me.ItemReady[=${CastThis}]}) {
						/if (${Me.Subscription.NotEqual[gold]} && ${FindItem[=${CastThis}].Prestige}) /return CAST_NO_RESULT
						/if (${IAmABard}) {
							/call CastBard "${CastThis}" ${TarID} CastItem
							/varset CastResult  ${Macro.Return}  
						} else {
							/useitem "${CastThis}" 
							/call DoCastingEvents 
							/while (${Me.Casting.ID}) {
								/delay 5
							}
							/call DoCastingEvents
							/varset CastResult ${castReturn}
						   |  Assign cast success if item on cooldown or summoned item consumed	
						}
						/if (!${Me.ItemReady[=${CastThis}]} || !${FindItem[=${CastThis}].ID} && !${Select[${CastResult},CAST_IMMUNE,CAST_RESISTED]})  /varset CastResult CAST_SUCCESS
						/if (${CastResult.Equal[CAST_SUCCESS]} || ${CastResult.Equal[CAST_RESIST]} || ${CastResult.Equal[CAST_TAKEHOLD]}) {
							/if (${DebugFramework}) /echo ## Item >> ${CastThis} << on ${TarID} from ${Array}. Cast Result ${CastResult}
							/if (${EQBCOn}) /call BroadCast ${EQBCOn} r "Item >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
							/if (${Array.NotEqual[Burn]}) /return ${CastResult}
						}
					} else /if (${CastThis.NotEqual[Feralist's Unity]} && ${Me.AltAbility[${CastThis}]} && ${Me.AltAbilityReady[${CastThis}]} && ${Me.AltAbility[${CastThis}].Type}!=5 && !${FindItem[=${CastThis}].ID})  { 
						
						/alt act ${Me.AltAbility[${CastThis}].ID}
						/call CastWait "${CastThis}"
						/if (${Me.AltAbilityReady[${CastThis}]}) {
							/varset CastResult CAST_RECOVER
						} else {
							/varset CastResult CAST_SUCCESS
						}
						/if (${CastResult.Equal[CAST_SUCCESS]} || ${CastResult.Equal[CAST_RESIST]} || ${CastResult.Equal[CAST_TAKEHOLD]}) {
							/if (${DebugFramework}) /echo ## AA >> ${CastThis} << on ${TarID} from ${Array}. Cast Result ${CastResult}
							/if (${EQBCOn}) /call BroadCast ${EQBCOn} r "AA >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
							/if (${Array.NotEqual[Burn]}) /return ${CastResult}
						}
					} else /if (${Me.CombatAbility[${CastThis}]} && !${Me.CombatAbilityTimer[${CastThis}]} && ${Me.CombatAbilityReady[${CastThis}]} && ${Spell[${CastThis}].EnduranceCost}<${Me.Endurance}) {
						|/echo !${Spell[${CastThis}].Duration} || (${Spell[${CastThis}].Duration} && ${Spell[${CastThis}].TargetType.Equal[Self]} && !${Me.ActiveDisc.ID}) || ${Spell[${CastThis}].TargetType.NotEqual[Self]} || ${Spell[${CastThis}].DurationWindow}
						|/if (!${Spell[${CastThis}].Duration} || (${Spell[${CastThis}].Duration} && ${Spell[${CastThis}].TargetType.Equal[Self]} && !${Me.ActiveDisc.ID}) || ${Spell[${CastThis}].TargetType.NotEqual[Self]} || ${Spell[${CastThis}].DurationWindow}) {
							|/varset WaitTimerCD2 30
							|/while (${Me.CombatAbilityReady[${CastThis}]} && ${WaitTimerCD2}) {
								/disc ${Me.CombatAbility[${Me.CombatAbility[${CastThis}]}].ID}
							|	/delay 5
							|}
							/varset CastResult CAST_SUCCESS
							/if (${DebugFramework}) /echo ## Disc >> ${CastThis} << on ${TarID} from ${Array}. Cast Result ${CastResult}
							/if (${EQBCOn} && (${CastResult.Equal[CAST_SUCCESS]} || ${CastResult.Equal[CAST_RESIST]} || ${CastResult.Equal[CAST_TAKEHOLD]})) /call BroadCast ${EQBCOn} r "Disc >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
						|}
						/if (${Array.NotEqual[Burn]}) /return ${CastResult}
					} else /if (${Me.Ability[${CastThis}]} && ${Me.AbilityReady[${CastThis}]}) {
						
						/doability "${CastThis}"
						/delay 20 !${Me.AbilityReady[${CastThis}]}
						/if (!${Me.AbilityReady[${CastThis}]}) {
							/if (${DebugFramework}) /echo ## Ability >> ${CastThis} << on ${TarID} from ${Array}. Cast Result ${CastResult}
							/if (${EQBCOn}) /call BroadCast ${EQBCOn} r "Ability >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
							/if (${Array.NotEqual[Burn]}) /return 
						}
						
						
					} else /if (${Me.SpellReady[${CastThis}]} && ${Me.Book[${CastThis}]} && ${Spell[${CastThis}].Mana}<${Me.CurrentMana}) {
						/if (${Me.Class.ShortName.Equal[BRD]}) {
							/call CastBard "${CastThis}" ${TarID} CastSpell
							/if (${EQBCOn} && (${CastResult.Equal[CAST_SUCCESS]} || ${CastResult.Equal[CAST_RESIST]} || ${CastResult.Equal[CAST_TAKEHOLD]})) /call BroadCast ${EQBCOn} r "Spell >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
							/if (${Array.NotEqual[Burn]}) /return ${CastResult}  
						} else /if (!${Target.CanSplashLand} && ${Spell[${CastThis}].TargetType.Equal[Free Target]}) {
						   /if (${DebugFramework}) /echo Splash Spell will not cast at targets location. Skipping ${CastThis}
						   /if (${Array.NotEqual[Burn]}) /return
						} else /if (${Me.Gem[${CastThis}]}) {
							
							:Retry_Cast
							/if (${Me.GemTimer[${CastThis}]}==0) {
								/cast "${CastThis}"
								/call DoCastingEvents 
								/varcalc TryNum ${TryNum}+1
								/while (${Me.Casting.ID} && ${CastResult.NotEqual[CAST_CANCELLED]}) {
									/if (${CastingInterruptOn}) {
										/if (${Array.Equal[Heals]}) {
										   /call CastInteruptHeals "${CastThis}"
										} else /if (${Select[${Array},dps,gom,burn]}>0) {
										   /call CastInteruptDPS "${CastThis}"
										} else /if (${Select[${Array},buffs,buffs-nomem]}>0) {
										   /call CastInteruptBuffs "${CastThis}"
										}
									}
									/delay 1
								}
								/call CastWait "${CastThis}"
								/call DoCastingEvents 
								
								/if (${Array.Equal[Pull]} && ${PullAggroTargetID}) /varset CastResult CAST_SUCCESS
								/if (${CastResult.NotEqual[CAST_CANCELLED]} || ${CastResult.NotEqual[CAST_NO_RESULT]}) /varset CastResult ${castReturn}  
							}
							/if (!${Me.SpellReady[${CastThis}]}) /varset CastResult CAST_SUCCESS
							
							/if (${CastResult.Equal[CAST_SUCCESS]} || ${CastResult.Equal[CAST_RESIST]} || ${CastResult.Equal[CAST_TAKEHOLD]}) {
								/if (${DebugFramework}) /echo ${CastResult} Returning ${CastThis} from ${Array}
								/if (${EQBCOn}) /call BroadCast ${EQBCOn} r "Spell >> ${CastThis} << on ${Spawn[id ${TarID}].CleanName} (${TarID}) from NewCast"
								/if (${Array.NotEqual[Burn]}) /return ${CastResult}
							}
							/if (${TryNum}<=${MaxTryNum} && (${CastResult.Equal[CAST_RESTART]} || ${CastResult.Equal[CAST_STUNNED]} || ${CastResult.Equal[CAST_FIZZLE]} || ${CastResult.Equal[CAST_COLLAPSE]} || ${CastResult.Equal[CAST_INTERRUPTED]} || ${CastResult.Equal[CAST_RESISTED]})) /goto :Retry_Cast
						} else {
							/if (${DebugFramework}) /echo Skip Casting ${CastThis}. Spell Not Memed.
							/varset CastResult CAST_NO_RESULT
						}
					} else {
						/if (${DebugFramework}) /echo in the cast garbage bin ${CastThis}
					}
					
				}
				/break
			}
		/next i
		/if (${DebugCombat}) /echo \atDEBUG NewCast leave. \agLine#: ${Macro.CurLine}
	/return

|----------------------------------------------------------------------------
|- SUB: FindTarget - melee attack portion removed. Melee attack moved into engage sub to reduce too far away and can not see messages
|-|${Ini[${InfoFileName},${ZoneName},MezImmune].Find[${Spawn[xtarhater named]}]}
|---------------------------------------------------------------------------- 
	
	Sub NewFindTarget(int callfrom)
		/if (${DebugCombat}) /echo \atDEBUG FindTarget Enter Callfrom: ${callfrom} \agLine#: ${Macro.CurLine}
		/declare TempTargetID int local
		/varset MyTargetID 0
		/varset MyTargetName NULL
		/if (!${Raid.Members}) {
			/if (${MainAssist.Equal[${Me}]}) {
				/if (${Spawn[xtarhater named].ID}) {
					/varset TempTargetID ${Spawn[xtarhater named].ID}
				} else /if (${Alert[4].Size} && ${Spawn[xtarhater alert 4].ID} && ${Spawn[xtarhater alert 4].Type.NotEqual[Corpse]}) {
					/varset TempTargetID ${NearestSpawn[1, xtarhater alert 4].ID}
				} else {
					/varset TempTargetID ${SpawnSort[1,asc,PctHPs,xtarhater].ID}
				}
				/call ValidateTarget ${TempTargetID}
				/if (${ValidTarget}==0) {
					/if (${Target.ID}) /squelch /alert add 2 id ${Target.ID}
					/varset MyTargetID 0
					/varset MyTargetName NULL
					/echo Invalid Target: ${Spawn[id ${TempTargetID}].CleanName} ${TempTargetID}
					/return
				} else {
					/if (${Target.ID}!=${TempTargetID}) {
					   /squelch /target clear
					   /delay 10 !${Target.ID}
					   /target id ${TempTargetID}
					   /delay 10 ${Target.ID}
					}
				   /varset MyTargetID ${Target.ID}
				   /varset MyTargetName ${Target.CleanName}
				   /echo Valid Target: ${Target.CleanName} ${Target.ID}
				}
			} else {
				|/if (${Me.XTarget[1].TargetType.NotEqual[Group Assist Target]} && ${Group.Members}) /xtarget set 1 groupassisttarget
				|/if (!${Bool[${Spawn[${MainAssist}].Dead}]}) /xtarget assist ${Spawn[${Spawn[${MainAssist}].CleanName}].ID}
				/if (!${Bool[${Spawn[${MainAssist}].Dead}]}) {
					/echo Assisting ${MainAssist}
					/assist ${MainAssist}
					/delay 1s ${Me.AssistComplete}
					/varset TempTargetID ${Target.ID}
					|/if (${Me.XTarget[1].TargetType.NotEqual[Group Assist Target]} && ${Group.Members}) /xtarget set 1 groupassisttarget
					|/xtarget assist ${Spawn[${Spawn[${MainAssist}].CleanName}].ID}
					|/xtarget assist ${Spawn[${MainAssist}].ID}
				} else /if (${Spawn[xtarhater named].ID}) {
					/varset TempTargetID ${Spawn[xtarhater named].ID}
				} else /if (${Alert[4].Size} && ${Spawn[xtarhater alert 4].ID} && ${Spawn[xtarhater alert 4].Type.NotEqual[Corpse]}) {
					/varset TempTargetID ${NearestSpawn[1, xtarhater alert 4].ID}
				} else {
					/varset TempTargetID ${SpawnSort[1,asc,PctHPs,xtarhater].ID}
				}
				|/delay 10 ${Me.XTarget[1].ID}
				|/call ValidateTarget ${Me.XTarget[1].ID}
				/call ValidateTarget ${TempTargetID}
				/if (${ValidTarget}==0) {
					/if (${Target.ID}) /squelch /alert add 2 id ${Target.ID}
					/varset MyTargetID 0
					/varset MyTargetName
					/xtarget set 1 autohater
					|/echo Invalid Target: ${Me.XTarget[1].CleanName} ${Me.XTarget[1].ID}
					/echo Invalid Target: ${MyTargetName} ${MyTargetID}
					/return
				} else {
					/varset MyTargetID ${TempTargetID}
					/varset MyTargetName ${Spawn[id ${TempTargetID}].CleanName}
					/echo Valid Target: ${MyTargetName} ${MyTargetID}
					|/varset MyTargetID ${Me.XTarget[1].ID}
					|/varset MyTargetName ${Me.XTarget[1].CleanName}
					|/echo Valid Target: ${MyTargetName} ${MyTargetID}
				}
			}
		} else /if (${Raid.Members}) {
			|/if (${Spawn[pc ${MA1}].ID} == ${Me.ID}) /call Bind_setCombatControl MA1 FALSE
			|/if (${Spawn[pc ${MA2}].ID} == ${Me.ID}) /call Bind_setCombatControl MA2 FALSE
			|/if (${Spawn[=${Raid.MainAssist} pc].ID} == ${Me.ID}) /call validate_assist NOECHO X
			|/if (${Bool[${Spawn[${Group.MainAssist}].Dead}]}) /goto :skippingtoMAtarget
			
			/if (!${Bool[${Spawn[${SplitAssist1_1}].Dead}]} && ${SplitAssist1_1.NotEqual[NULL]}) {
				/xtarget assist ${Spawn[${Spawn[${SplitAssist1_1}].CleanName}].ID}
			} else /if (!${Bool[${Spawn[${SplitAssist1_2}].Dead}]} && ${SplitAssist1_2.NotEqual[NULL]}) {
				/xtarget assist ${Spawn[${Spawn[${SplitAssist1_2}].CleanName}].ID}
			} else /if (!${Bool[${Spawn[${SplitAssist2_1}].Dead}]} && ${SplitAssist2_1.NotEqual[NULL]}) {
				/xtarget assist ${Spawn[${Spawn[${SplitAssist2_1}].CleanName}].ID}
			} else /if (!${Bool[${Spawn[${SplitAssist2_2}].Dead}]} && ${SplitAssist2_2.NotEqual[NULL]}) {
				/xtarget assist ${Spawn[${Spawn[${SplitAssist2_2}].CleanName}].ID}
			} else /if (!${Bool[${Spawn[${MainAssist}].Dead}]}) {
				|/xtarget assist ${Spawn[${Spawn[${MainAssist}].CleanName}].ID}
				/assist ${MainAssist}
			} else /if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 1 Target]}) {
				/xtarget set 1 raidassist1target
			} 
			/delay 1s ${Me.AssistComplete}
			/varset TempTargetID ${Target.ID}
			|else /if (${useSmartAssist2} && ${Me.XTarget[1].TargetType.NotEqual[Raid Assist 2 Target]} && ${Raid.Members}) {
			|	/xtarget set 1 raidassist2target
			|} else /if (${useSmartAssist3} && ${Me.XTarget[1].TargetType.NotEqual[Raid Assist 3 Target]} && ${Raid.Members}) {
			|	/xtarget set 1 raidassist3target
			|}
			|/delay 10 ${Me.XTarget[1].ID}
			|/call ValidateTarget ${Me.XTarget[1].ID}
			/call ValidateTarget ${TempTargetID}
			/if (${ValidTarget}==0) {
				/if (${Target.ID}) /squelch /alert add 2 id ${Target.ID}
				/varset MyTargetID 0
				/varset MyTargetName
				/xtarget set 1 autohater
				/echo Invalid Target: ${Me.XTarget[1].CleanName} ${Me.XTarget[1].ID}
				/return
			} else {
				/varset MyTargetID ${Me.XTarget[1].ID}
				/varset MyTargetName ${Me.XTarget[1].CleanName}
				/echo Valid Target: ${MyTargetName} ${MyTargetID}
			}
		}

		/if (${DebugCombat}) /echo \atDEBUG FindTarget leave. \agLine#: ${Macro.CurLine}
	/return

|----------------------------------------------------------------------------
|- SUB: FindTarget - melee attack portion removed. Melee attack moved into engage sub to reduce too far away and can not see messages
|-|${Ini[${InfoFileName},${ZoneName},MezImmune].Find[${Spawn[xtarhater named]}]}
|---------------------------------------------------------------------------- 

	Sub FindTarget(int callfrom)
		/declare TempTargetID int local
		/if (${DebugCombat}) /echo \atDEBUG FindTarget Enter Callfrom: ${callfrom} \agLine#: ${Macro.CurLine}
		/varset MyTargetID 0
		/varset MyTargetName NULL
		/if (${Spawn[xtarhater named].ID}) {
			/varset TempTargetID ${Spawn[xtarhater named].ID}
			|/call BroadCast ${EQBCOn} y "Grabbing Namer -> ${Spawn[${TempTargetID}].Name} <- ID:${Spawn[${TempTargetID}].ID} "
		} else /if (${Alert[4].Size} && ${Spawn[xtarhater alert 4].ID} && ${Spawn[xtarhater alert 4].Type.NotEqual[Corpse]}) {
			/varset TempTargetID ${NearestSpawn[1, xtarhater alert 4].ID}
			|/call BroadCast ${EQBCOn} y "Grabbing MEZ Immune -> ${Spawn[${TempTargetID}].Name} <- ID:${Spawn[${TempTargetID}].ID} "
			
		} else {
			/varset TempTargetID ${SpawnSort[1,asc,PctHPs,xtarhater].ID}
			|/call BroadCast ${EQBCOn} y "Grabbing Lowest HP Mob -> ${Spawn[${TempTargetID}].Name} <- ID:${Spawn[${TempTargetID}].ID} "
		}
		
		/call ValidateTarget ${TempTargetID}
			
		/if (${ValidTarget}==0) {
			/if (${Target.ID}) /squelch /alert add 2 id ${Target.ID}
			/varset MyTargetID 0
			/varset MyTargetName
			/echo invalid target
			/return
		} else {
			/if (${Target.ID}!=${TempTargetID}) {
			   /squelch /target clear
			   /delay 10 !${Target.ID}
			   /target id ${TempTargetID}
			   /delay 10 ${Target.ID}
			}
		   /varset MyTargetID ${Target.ID}
		   /varset MyTargetName ${Target.CleanName}
		   /echo valid target ${Target.CleanName} ${Target.ID}
		}
		|/if (${IAmMA} && !${Raid.Members}) {
		|	/bc TANKING-> ${MyTargetName} <- ID:${MyTargetID}
		|}
	|--Check target matches MA if not based off lowest hp in XTarget
		/if (${Target.ID}!=${Me.GroupAssistTarget.ID} && ${Group.MainAssist.ID}!=${Me.ID} && ${MyTargetID}!=${Me.GroupAssistTarget.ID} && ${Spawn[id ${Me.GroupAssistTarget.ID} ${SpawnSearchType}].ID}) {
			/varset MyTargetID ${Me.GroupAssistTarget.ID}
		} 
		|else /if ((!${Group.MainAssist.ID} || ${Group.MainAssist.ID}==${Me.ID}) && ${Spawn[${SpawnSearchType} id ${SpawnSort[1,desc,PctHPs,xtarhater].ID}].ID}) {
		|	/varset MyTargetID ${Spawn[${SpawnSearchType} id ${SpawnSort[1,desc,PctHPs,xtarhater].ID}].ID}
		|}
	|--Target the new target
		|/if (!${DPSPaused} && !${Spawn[corpse id ${MyTargetID}].ID} && ${Spawn[targetable id ${MyTargetID}].ID} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].PctHPs}<=${AssistAt} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].Distance}<${MeleeDistance} && (!${Target.Mezzed.ID} || ${Select[${Role},manual,petassist,tank,pettank,puller,pullertank,pullerpettank,hunter,hunterpettank,offtank]})) {
		|	/if (${Target.ID}!=${MyTargetID} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].ID} && ${DPSOn} && ${MeleeOn}) {
		|		/squelch /target id ${MyTargetID}
		|		/delay 10 ${Target.ID}==${MyTargetID}
		|	}
		|	/call EngageTarget
		|}
		/if (${Target.ID}!=${MyTargetID} && ${Spawn[npc id ${MyTargetID}].ID}) {
			/squelch /target id ${MyTargetID}
			/delay 10 ${Target.ID}==${MyTargetID}
		}
		/if (${DebugCombat}) /echo \atDEBUG FindTarget leave. \agLine#: ${Macro.CurLine}
	/return 



| -------------------------------------------------------------------------------------
| SUB: watch_for_death
| -------------------------------------------------------------------------------------  

	Sub watch_for_death()
		|/echo in watch for death
		| check for current flags. watch for rez box if I have them
		/if (${IAmDead}) {
			/return TRUE
		}
		
		| check death states
		/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) {
			/if (${Me.Casting.ID}) /stopcast
			/return TRUE
		}
		
		/if (${Me.Buff[Resurrection Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) {
			/return TRUE
		}
		
	/return FALSE
	
	|----------------------------------------------------------------------------
|- SUB: Melee Engage Target - Need a StickHow variable in the mac. Ini option for melee just a declared variable set to null for classes that wont enageg in melee
|- Basic use will check the that the target is in assist range, stick and when in melee range start attacking ( reduces too far away and can not see messages )
|- Advanced use ( smart assist ) will check that the target is range of the MA before engaging....keeps melee from charging out after a mob on incoming
|- For classes that have a before combat move, it is recommended  to copy/paste this sub into new macro and rename. Add opening move ( ber, rog ) to just before turning on attack 
|---------------------------------------------------------------------------- 
	
	Sub EngageTarget
		/if (${DebugCombat}) /echo \atDEBUG EngageTarget Enter \agLine#: ${Macro.CurLine}
		/echo Engaging >> ${MyTargetID}
		/doevents
		/varset MercAssisting 0
		/varset CombatStart 1
		/if (${Me.State.Equal[FEIGN]} ||${Me.Sitting}) /stand
		/if (${Stick.Status.Equal[ON]}) /stick off
		/if (${IAmDead} && ${CampZone}==${Zone.ID} && (${Me.Buff[Resurrection Sickness].ID} || ${SpawnCount[pccorpse ${Me}]}==0) ) /varset IAmDead 0
		
		/if (${Target.ID}!=${MyTargetID} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].ID} && ${DPSOn}) {
			/squelch /target id ${MyTargetID}
			/delay 10 ${Target.ID}==${MyTargetID}
			|/if (${MeleeOn}) /nav id ${MyTargetID}
			/if (${MeleeOn}) /moveto id ${MyTargetID}
		}
		/if (!${Me.Combat} && ${BeforeArray[1].NotEqual[null]}) /call BeforeAttack ${MyTargetID} 2

		/if (${DPSOn} && ${MeleeOn} && !${Attacking}) {
			/varset Attacking 1
			/if (!${AutoFireOn}) {
				/if (!${UseMQ2Melee} && ${MeleeOn}) {
						/if (${Spawn[${MyTargetID}].Distance}>13) {
						/moveto id ${MyTargetID}
					}
					/if (${Me.FeetWet}) {
						/stick 8 uw ${StickHow} id ${MyTargetID}
					} else {
						/stick 8 ${StickHow} id ${MyTargetID}
					}	
				} else /killthis
				| Move closer if Z distance more than 4 due to levitation
				/call ZAxisCheck ${ZDist} 4.1
				/if (!${Me.Combat} && ${BeforeArray[1].NotEqual[null]}) /call BeforeAttack ${MyTargetID} 1
			}
			/squelch /attack on
			/delay 10 ${Me.Combat}
			/if (${Cursor.ID}) /call CheckCursor Combat
		} else /if (${DPSOn} && ${AutoFireOn} && !${Me.AutoFire} && ${Target.ID}==${MyTargetID} && !${Bool[${Target.Mezzed.ID}]}) {
			/echo turning on autofire
			/if (${Target.Distance3D} < 30) {
				/echo need to move back a little
				/stick 40 moveback
				/delay 2s
				/stick off
			}
			/autofire on
		}
		
		/echo  ATTACKING -> ${Spawn[${MyTargetID}].CleanName} <-

		
		/if (${Select[${Role},pettank,pullerpettank]} && !${Attacking}) { 
			/pet attack
			/pet swarm
			/varset Attacking 1
		}
		
		/if (${Select[${Zone.ID},795,796]}!=0) {
			/varset tmpHealOverride 85.00
		}
		
		|COMBAT LOOP
		
		/while (1) {
			|/bc ${MyTargetID}
			AMIDEAD
			/if (${DebugCombat}) /echo \atDEBUGCOMBATWHILE Attack 1 \agLine#: ${Macro.CurLine}
			/varcalc cyclecount (${cyclecount})+1
			/doevents
			
			

			/if (${Role.Equal[offtank]} && !${Spawn[=${MainAssist}].ID}) /switchma ${Me} tank 0
			/if (${DebuffAllOn} && (${Role.NotEqual[puller]} || !${ChainPull})) {
			   /call DoDebuffStuff ${MyTargetID}
				/if (${Spawn[${MyTargetID}].Type.Equal[Corpse]} || !${Spawn[${MyTargetID}].ID} || ${DPSPaused}) {
				   /call CombatReset 0 combat
				   /break
				}
			} 
			
			/call _${Me.Class.ShortName}
			|/echo past class sub call
			/if (${Target.ID}!=${MyTargetID} && ${Spawn[${SpawnSearchType} id ${MyTargetID}].ID} && ${DPSOn} && ${MeleeOn}) /squelch /target id ${MyTargetID}
			/if (${Spawn[${MyTargetID}].Distance}>${Spawn[${MyTargetID}].MaxRangeTo} && ${DPSOn} && ${MeleeOn} && !${Stick.Active}) {
				/moveto id ${MyTargetID} mdist ${Spawn[${MyTargetID}].MaxRangeTo}
			} else /if (${DPSOn} && ${AutoFireOn} && !${Me.AutoFire} && ${Target.ID}==${MyTargetID} && !${Bool[${Target.Mezzed.ID}]}) {
				/echo turning on autofire
				/if (${Target.Distance3D} < 30) {
					/echo need to move back a little
					/stick 40 moveback
					/delay 2s
					/stick off
				}
				/autofire on
			}
			/if (${MeleeOn} && !${Me.Combat} && ${Target.ID}) /attack on
			/if (!${NamedCheck} && ((${BurnAllNamed} && ${Spawn[xtarhater named].ID} )||(!${BurnAllNamed} && ${Ini[${InfoFileName},${ZoneName},MobsToBurn].Find[${MyTargetName}]}))) {
				/popup *** Mob:(${Target.CleanName}) is a NAMED!
				/if (${UseTribute} && !${Me.TributeActive}) {
				   /squelch /tribute personal on
				   
				}
				/varset NamedCheck 1
				/call NewCast ${MyTargetID} "Burn"
			}
			/if (${checkmana} && ${Me.PctMana}<65) /call CastMana combat
			/call WriteDebuffs
			/if (${CuresOn}) /call CheckCures
			
			/if (${Role.Equal[puller]} && ${ChainPull}) {
				/if (${Math.Distance[${CampYLoc},${CampXLoc}:${Spawn[=${MainAssist}].Y},${Spawn[=${MainAssist}].X}]}<=75) {
					/if (!${PullHold} && ${MobCount}<2 && !${Me.XTarget[${XTSlot2}].ID} && ${If[${Target.Named},0,1]}==1) {
						/if (${Target.ID} && ${Target.PctHPs} < ${ChainPullHP}) {
							/call FindMobToPull 0
							/if (${Macro.Return}>0 && (!${PullXPCheck} || (${Spawn[${ChainPullTemp}].Distance}<${Math.Calc[${PullRange}+400]} && ${PullXPCheck})) && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]} && ${Me.PctAggro}<50) {
								/varset PullMob ${Macro.Return}
								/if (${UseMQ2Melee}) /squelch /melee melee=0
								/if (${Stick.Active}) /squelch /stick off 
								/squelch /attack off
								/delay 10
								/if (${UseMQ2Melee}) /squelch /melee melee=1
								/varset ChainPull 2
								/varset MyTargetID 0
								/varset Attacking 0
								/return
							}
						}
					}
				} else /if (${SpamTankWait}) {
					/call BroadCast ${EQBCOn} r "Holding Pulls. Tank to far from camp."
					/varset SpamTankWait 0 
				}
				/if (${Role.Equal[puller]} && ${ChainPull} && ${DebuffAllOn}) /call DebuffCast ${MyTargetID} 1 
				
			}   
			
			/if (${IAmDead} || ${DPSPaused} || ${MyTargetID}==0 || ${Spawn[corpse id ${MyTargetID}].ID} || !${Spawn[targetable id ${MyTargetID}].ID} || ${Me.Feigning} ||  (${Me.XTarget[1].ID} > 0 && ${Spawn[${SpawnSearchType} id ${MyTargetID}].Distance}>${MeleeDistance}) ) /break

			
		}
		
		/call CombatReset 0 combat
		/varset MyTargetID 0
		/varset healcount 0
		/varset CombatStart 0
		/xtarget set 1 autohater
		/if (${UseTribute} && ${Me.TributeActive}) {
		   /squelch /tribute personal off
		   
		}
		
		/if (${cyclecount}>0) {
			/if (${DebugFramework}) /echo CYCLES: ${cyclecount}
			/varset cyclecount 0
		}
		
		/varset tmpHealOverride 99.99
		/if (${DebugCombat}) /echo \atDEBUG EngageTarget leave. \agLine#: ${Macro.CurLine}
	/return
	
	
	


|----------------------------------------------------------------------------
|- SUB: FindTarget - melee attack portion removed. Melee attack moved into engage sub to reduce too far away and can not see messages
|-|${Ini[${InfoFileName},${ZoneName},MezImmune].Find[${Spawn[xtarhater named]}]}
|---------------------------------------------------------------------------- 

	Sub CastCheck(CheckThis)
		/if (${FindItem[=${CheckThis}].ID} && ${Me.ItemReady[=${CheckThis}]}) /return TRUE
		/if (${Me.AltAbility[${CheckThis}]} && ${Me.AltAbilityReady[${CheckThis}]} && ${Me.AltAbility[${CheckThis}].Type}!=5 && !${FindItem[=${CheckThis}].ID}) /return TRUE
		/if (${Me.CombatAbility[${CheckThis}]} && !${Me.CombatAbilityTimer[${CheckThis}]} && ${Me.CombatAbilityReady[${CheckThis}]} && ${Spell[${CheckThis}].EnduranceCost}<${Me.Endurance}) /return TRUE
		/if (${Me.Ability[${CheckThis}]} && ${Me.AbilityReady[${CheckThis}]}) /return TRUE
		/if (${Me.SpellReady[${CheckThis}]} && ${Me.Book[${CheckThis}]} && ${Spell[${CheckThis}].Mana}<${Me.CurrentMana}) /return TRUE
		/if (${CheckThis.Find[command:]}) /return TRUE
		/if (${CheckThis.Find[script]}) /return TRUE
	/return FALSE
	
	
#event RoS1Run                     "#*#My spines will find a target, #1# and #2#.#*#"


Sub Event_RoS1Run(string line, string p1, string p2)

    /if (${Me.Name.Equal[${p1}]} || ${Me.Name.Equal[${p2}]}) {
        /backoff
        /stick pause lock
        /nav wp run
        /delay 12s
        /nav wp back
        /stick unpause lock
        /backoff
    }
/return