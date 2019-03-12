
sub watch_for_death()
	/if (${IAmDead}) /return TRUE
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return TRUE
	/if (${Me.Buff[Resurrection Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) /return TRUE
/return FALSE

Sub CastCheck(CheckThis)
	/if (${FindItem[=${CheckThis}].ID} && ${Me.ItemReady[=${CheckThis}]}) /return TRUE
	/if (${Me.AltAbility[${CheckThis}]} && ${Me.AltAbilityReady[${CheckThis}]} && ${Me.AltAbility[${CheckThis}].Type}!=5 && !${FindItem[=${CheckThis}].ID}) /return TRUE
	/if (${Me.CombatAbility[${CheckThis}]} && !${Me.CombatAbilityTimer[${CheckThis}]} && ${Me.CombatAbilityReady[${CheckThis}]} && ${Spell[${CheckThis}].EnduranceCost}<${Me.Endurance}) /return TRUE
	/if (${Me.Ability[${CheckThis}]} && ${Me.AbilityReady[${CheckThis}]}) /return TRUE
	/if (${Me.SpellReady[${CheckThis}]} && ${Me.Book[${CheckThis}]} && ${Spell[${CheckThis}].Mana}<${Me.CurrentMana}) /return TRUE
	/if (${CheckThis.Find[command:]}) /return TRUE
	/if (${CheckThis.Find[script]}) /return TRUE
/return FALSE

sub validate_target(bool _error, int _tmpID)

	AMIDEAD
		
	/if (!${Target.ID}) /return FALSE
	/if (!${Bool[${_tmpID}]}) /return FALSE
	/if (${SafeZone} && !${switchOverrideSafezone}) /return FALSE

	| check smart assists
	/if (!${useForceAssist}) {
		
		/if (${Target.ID}) {
			/if ((${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}) && ${Spawn[${_tmpID}].ID} != ${Me.XTarget[1].ID}) {
				/call clear_combat
				/return FALSE
			} else /if (${useSmartAssistG} && ${Group.MainAssist.ID} && !${Bool[${Spawn[${Group.MainAssist}].Dead}]} && ${Spawn[${_tmpID}].ID} != ${Me.XTarget[1].ID}) {
				/call clear_combat
				/return FALSE
			}
		}
		
		/if (!${switchOverrideEngageat} && !${switchPull}) {
			/if (${Spawn[${_tmpID}].PctHPs} > ${engageAt} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG}) {
				/delay 1
				/return FALSE
			} else /if (${Spawn[${_tmpID}].PctHPs} > ${smartengageAt} && (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}|| ${useSmartAssistG})) {
				/delay 1
				/return FALSE
			}
		}
	}
	
	| shit we really shouldnt't be attacking
	/if (${Select[${Spawn[${_tmpID}].Type},AURA,BANNER,CAMPFIRE,CORPSE,CHEST,ITEM,TRIGGER,TRAP,TIMER,MOUNT,Mercenary]}) /return FALSE
	| If mob found on ignore and not on xtarget skip
	/if (${MobsToIgnore.Find[${MobName}]} && ${Select[${MobID},${Me.XTarget[1].ID},${Me.XTarget[2].ID},${Me.XTarget[3].ID},${Me.XTarget[4].ID},${Me.XTarget[5].ID},${Me.XTarget[6].ID},${Me.XTarget[7].ID},${Me.XTarget[8].ID},${Me.XTarget[9].ID},${Me.XTarget[10].ID},${Me.XTarget[11].ID},${Me.XTarget[12].ID},${Me.XTarget[13].ID}]}==0) /return MobOnIgnoreList
	/if (${MobsToIgnoreByID.Find[${MobID}|]}) /return MobOnIgnoreListByID
	|/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /return FALSE
	/if (${Target.ID} && ${Bool[${Target.Mezzed.Duration}]} && !${switchPull}) /return FALSE
	/if ((!${Spawn[${_tmpID}].LineOfSight} && !${switchOverrideLos}) || ${Spawn[${_tmpID}].Distance} > ${MobAgro}) /return FALSE
	/if (${Spawn[${_tmpID}].Type.Equal[PC]}) /return FALSE
	/if (${Target.Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${_tmpID} == ${Me.ID} || ${_tmpID} == ${Me.Pet.ID}) /return FALSE

	| /if (${Select[${Me.Class.ShortName},ROG]} && ${switchSos}) /makemevisible

/return TRUE
| -------------------------------------------------------------------------------------
| SUB: Validate Target Combined Spawn and Target
| -------------------------------------------------------------------------------------
    Sub OLDValidateTarget(int SpawnID)
        /if (${DebugCombat}) /echo \atDEBUGCOMBAT ValidateTarget Enter Target Name:${Target.CleanName} ID:${Target.ID} Spawn Name: ${Spawn[${SpawnID}].CleanName} ID:${SpawnID} \agLine#: ${Macro.CurLine}
        /declare j int local
        /declare PullValid int local 0
        /declare MobID int local 0
        /declare MobName string local
        /declare MobType string local
        /declare MeleeDistanceCheck int local ${MeleeDistance}       
        /varset ValidTarget 0
        /if (${SpawnID}) {
           /varset MobID ${Spawn[${SpawnID}].ID}
           /varset MobName ${Spawn[${SpawnID}].CleanName}
           /varset MobType ${Spawn[${SpawnID}].Type}
        } else {
           /varset MobID ${Target.ID}
           /varset MobName ${Target.CleanName}
           /varset MobType ${Target.Type}
        }
        /if (!${MobID}) /return NoTarget
        /if (${DebugCombat}) /echo \atDEBUGCOMBAT ValidateTarget Validate 1 \agLine#: ${Macro.CurLine}
        /if (${Select[${MobType},AURA,BANNER,CAMPFIRE,CORPSE,CHEST,ITEM,TRIGGER,TRAP,TIMER,MOUNT,Mercenary]}) /return BadTargetType
        /if (${DebugCombat}) /echo \atDEBUGCOMBAT ValidateTarget Validate 2 \agLine#: ${Macro.CurLine}
        | If mob found on ignore and not on xtarget skip
        /if (${MobsToIgnore.Find[${MobName}]} && ${Select[${MobID},${Me.XTarget[1].ID},${Me.XTarget[2].ID},${Me.XTarget[3].ID},${Me.XTarget[4].ID},${Me.XTarget[5].ID},${Me.XTarget[6].ID},${Me.XTarget[7].ID},${Me.XTarget[8].ID},${Me.XTarget[9].ID},${Me.XTarget[10].ID},${Me.XTarget[11].ID},${Me.XTarget[12].ID},${Me.XTarget[13].ID}]}==0) /return MobOnIgnoreList
        /if (${MobsToIgnoreByID.Find[${MobID}|]}) /return MobOnIgnoreListByID
        | Check if target is somewhere on my extended target.        
        /if (${Select[${Role},tank]} && ${MobCount}<=${XSlotTotal} && ${Spawn[${MainAssist} ${MainAssistType} group].ID}) {
            /if (${SpawnCount[id ${MobID} xtarhater]}==0) /return NotOnXTarget
        }
        /if (${Spawn[id ${MobID}].MaxRangeTo}>${MeleeDistance}) /varcalc MeleeDistanceCheck ${Spawn[id ${MobID}].MaxRangeTo}+5
        /if (${ReturnToCamp} && ${Math.Distance[${Spawn[${MobID}].Y},${Spawn[${MobID}].X}:${CampYLoc},${CampXLoc}]}>${MeleeDistanceCheck} && !${Pulling} && ${Select[${Role},tank,pullertank,pettank,pullerpettank]}) /return OutofCampRadius
        /if (${DebugCombat}) /echo \atDEBUGCOMBAT ValidateTarget Validate 3 \agLine#: ${Macro.CurLine}
        /if (${MobName.Find[eye of]} && ${SpawnCount[pc ${MobName.Right[${Math.Calc[${MobName.Length}-7]}]}]}) /return Spell-Eye-PC        
        /if (${MobType.Equal[Pet]} && ${Spawn[${MobID}].Master.Type.Equal[PC]}) /return PET-PC
        /if (!${Select[${EverQuest.Server},zek]} && ${Select[${MobType},PC]}) /return PC
        | This prevents characters on the Zek server from attacking PC targets in their group
        /if (${Select[${EverQuest.Server},zek]} && ${Select[${MobType},PC]}) {
            /if (${Spawn[id ${MobID} group].ID}>0) /return GroupMember
            /if (${MobID}==${Spawn[=${MainAssist}].ID}) /return MA
        }
        /if (${DebugCombat}) /echo \atDEBUGCOMBAT ValidateTarget Validate 4 \agLine#: ${Macro.CurLine}
        | Only valid when pulling
        /if (${Pulling} && !${Pulled}) {
            /if ((${Select[${PullMoveUse},los,nav]}>=1 && ${Math.Distance[${Spawn[${MobID}].Y},${Spawn[${MobID}].X}:${CampYLoc},${CampXLoc}]}>${MaxRadius})) /return OutofRadius
            /if (${Select[${Role},puller,pullertank,pullerpettank]} && !${Spawn[${MobID}].LineOfSight} && ${PullMoveUse.Equal[los]}) /return NoLOS            
            /if (${SpawnCount[loc ${Spawn[${MobID}].X} ${Spawn[${MobID}].Y} radius 12 pc]}>=1 && ${Pulling} && ${Math.Distance[${Spawn[${MobID}].Y},${Spawn[${MobID}].X}:${Me.Y},${Me.X}]}>=16) /return PCNear
            /if (${Spawn[${MobID}].PctHPs}<=99) /return PullNotFullHealth
            /if (${Spawn[${MobID}].Level}<${PullMin} || ${Spawn[${MobID}].Level}>${PullMax}) /return BadLevel 
            /if (${Window[TargetOfTargetWindow].Open} && ${Target.ID}==${MobID}) {
            | - If mob I'm pulling is aggroed already by non damaging spell by someone else.
               /delay 10 ${Target.ID}==${MobID}
               /if (${Me.TargetOfTarget.Type.Equal[PC]} && ${Me.TargetOfTarget.Name.NotEqual[${Me}]} && ${Group.Member[${Me.TargetOfTarget.Name}].Index}<1) /return PullToTTNotPuller
               /if (${Me.TargetOfTarget.Type.Equal[PET]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.Pet.CleanName}]}) /return PullToTTNotMyPet
            }
            /if (${DebugPull} && !${MobsToPull.Find[null]}) /echo \atDEBUGPULL ValidateTarget ${MobsToPull} \agLine#: ${Macro.CurLine}
            /if (${MobsToPull.Find[ALL for all]} || ${MobsToPull.Equal[ALL]}) {
                /varset PullValid 1
            } else {
                /for j 1 to 25
                    /if (${DebugPull}) /echo \atDEBUGPULL ValidateTarget ${j} ${MobsToPull.Arg[${j},,]} ${MobName.Find[${MobsToPull.Arg[${j},,]}]} \agLine#: ${Macro.CurLine}
                    /if (${MobName.Find[${MobsToPull.Arg[${j},,]}]}) {
                        /varset PullValid 1
                        /break
                    }
                /next j
            }
            /if (!${PullValid}) /return PullMobNotonList
        }
        /varset ValidTarget 1
        /if (${DebugCombat}) /echo \atDEBUGCOMBAT ValidateTarget Leave \agLine#: ${Macro.CurLine}
    /return ${ValidTarget}