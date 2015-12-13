/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */
#ifndef _ARMV8REG_H_
#define	_ARMV8REG_H_

#ifdef __ASM__
#define DLR_EL0_get(v)                              MRS v,DLR_EL0
#define DLR_EL0_set(v)                              MSR DLR_EL0,v
#define DSPSR_EL0_get(v)                            MRS v,DSPSR_EL0
#define DSPSR_EL0_set(v)                            MSR DSPSR_EL0,v
#define ELR_EL1_get(v)                              MRS v,ELR_EL1
#define ELR_EL1_set(v)                              MSR ELR_EL1,v
#define ELR_EL2_get(v)                              MRS v,ELR_EL2
#define ELR_EL2_set(v)                              MSR ELR_EL2,v
#define ELR_EL3_get(v)                              MRS v,ELR_EL3
#define ELR_EL3_set(v)                              MSR ELR_EL3,v
#define FPCR_get(v)                                 MRS v,FPCR
#define FPCR_set(v)                                 MSR FPCR,v
#define FPSR_get(v)                                 MRS v,FPSR
#define FPSR_set(v)                                 MSR FPSR,v
#define SP_EL0_get(v)                               MRS v,SP_EL0
#define SP_EL0_set(v)                               MSR SP_EL0,v
#define SP_EL1_get(v)                               MRS v,SP_EL1
#define SP_EL1_set(v)                               MSR SP_EL1,v
#define SP_EL2_get(v)                               MRS v,SP_EL2
#define SP_EL2_set(v)                               MSR SP_EL2,v
#define SP_EL3_get(v)                               MRS v,SP_EL3
#define SP_EL3_set(v)                               MSR SP_EL3,v
#define SPSR_abt_get(v)                             MRS v,SPSR_abt
#define SPSR_abt_set(v)                             MSR SPSR_abt,v
#define SPSR_EL1_get(v)                             MRS v,SPSR_EL1
#define SPSR_EL1_set(v)                             MSR SPSR_EL1,v
#define SPSR_EL2_get(v)                             MRS v,SPSR_EL2
#define SPSR_EL2_set(v)                             MSR SPSR_EL2,v
#define SPSR_EL3_get(v)                             MRS v,SPSR_EL3
#define SPSR_EL3_set(v)                             MSR SPSR_EL3,v
#define SPSR_fiq_get(v)                             MRS v,SPSR_fiq
#define SPSR_fiq_set(v)                             MSR SPSR_fiq,v
#define SPSR_irq_get(v)                             MRS v,SPSR_irq
#define SPSR_irq_set(v)                             MSR SPSR_irq,v
#define SPSR_und_get(v)                             MRS v,SPSR_und
#define SPSR_und_set(v)                             MSR SPSR_und,v
#define AMAIR_EL1_get(v)                            MRS v,AMAIR_EL1
#define AMAIR_EL1_set(v)                            MSR AMAIR_EL1,v
#define AMAIR_EL2_get(v)                            MRS v,AMAIR_EL2
#define AMAIR_EL2_set(v)                            MSR AMAIR_EL2,v
#define AMAIR_EL3_get(v)                            MRS v,AMAIR_EL3
#define AMAIR_EL3_set(v)                            MSR AMAIR_EL3,v
#define CONTEXTIDR_EL1_get(v)                       MRS v,CONTEXTIDR_EL1
#define CONTEXTIDR_EL1_set(v)                       MSR CONTEXTIDR_EL1,v
#define DACR32_EL2_get(v)                           MRS v,DACR32_EL2
#define DACR32_EL2_set(v)                           MSR DACR32_EL2,v
#define MAIR_EL1_get(v)                             MRS v,MAIR_EL1
#define MAIR_EL1_set(v)                             MSR MAIR_EL1,v
#define MAIR_EL2_get(v)                             MRS v,MAIR_EL2
#define MAIR_EL2_set(v)                             MSR MAIR_EL2,v
#define MAIR_EL3_get(v)                             MRS v,MAIR_EL3
#define MAIR_EL3_set(v)                             MSR MAIR_EL3,v
#define TCR_EL1_get(v)                              MRS v,TCR_EL1
#define TCR_EL1_set(v)                              MSR TCR_EL1,v
#define TCR_EL2_get(v)                              MRS v,TCR_EL2
#define TCR_EL2_set(v)                              MSR TCR_EL2,v
#define TCR_EL3_get(v)                              MRS v,TCR_EL3
#define TCR_EL3_set(v)                              MSR TCR_EL3,v
#define TTBR0_EL1_get(v)                            MRS v,TTBR0_EL1
#define TTBR0_EL1_set(v)                            MSR TTBR0_EL1,v
#define TTBR0_EL2_get(v)                            MRS v,TTBR0_EL2
#define TTBR0_EL2_set(v)                            MSR TTBR0_EL2,v
#define TTBR0_EL3_get(v)                            MRS v,TTBR0_EL3
#define TTBR0_EL3_set(v)                            MSR TTBR0_EL3,v
#define TTBR1_EL1_get(v)                            MRS v,TTBR1_EL1
#define TTBR1_EL1_set(v)                            MSR TTBR1_EL1,v
#define VTCR_EL2_get(v)                             MRS v,VTCR_EL2
#define VTCR_EL2_set(v)                             MSR VTCR_EL2,v
#define VTTBR_EL2_get(v)                            MRS v,VTTBR_EL2
#define VTTBR_EL2_set(v)                            MSR VTTBR_EL2,v
#define AIDR_EL1_get(v)                             MRS v,AIDR_EL1
#define CCSIDR_EL1_get(v)                           MRS v,CCSIDR_EL1
#define CLIDR_EL1_get(v)                            MRS v,CLIDR_EL1
#define CSSELR_EL1_get(v)                           MRS v,CSSELR_EL1
#define CSSELR_EL1_set(v)                           MSR CSSELR_EL1,v
#define CTR_EL0_get(v)                              MRS v,CTR_EL0
#define DCZID_EL0_get(v)                            MRS v,DCZID_EL0
#define ID_AA64AFR0_EL1_get(v)                      MRS v,ID_AA64AFR0_EL1
#define ID_AA64AFR1_EL1_get(v)                      MRS v,ID_AA64AFR1_EL1
#define ID_AA64DFR0_EL1_get(v)                      MRS v,ID_AA64DFR0_EL1
#define ID_AA64DFR1_EL1_get(v)                      MRS v,ID_AA64DFR1_EL1
#define ID_AA64ISAR0_EL1_get(v)                     MRS v,ID_AA64ISAR0_EL1
#define ID_AA64ISAR1_EL1_get(v)                     MRS v,ID_AA64ISAR1_EL1
#define ID_AA64MMFR0_EL1_get(v)                     MRS v,ID_AA64MMFR0_EL1
#define ID_AA64MMFR1_EL1_get(v)                     MRS v,ID_AA64MMFR1_EL1
#define ID_AA64PFR0_EL1_get(v)                      MRS v,ID_AA64PFR0_EL1
#define ID_AA64PFR1_EL1_get(v)                      MRS v,ID_AA64PFR1_EL1
#define ID_AFR0_EL1_get(v)                          MRS v,ID_AFR0_EL1
#define ID_DFR0_EL1_get(v)                          MRS v,ID_DFR0_EL1
#define ID_ISAR0_EL1_get(v)                         MRS v,ID_ISAR0_EL1
#define ID_ISAR1_EL1_get(v)                         MRS v,ID_ISAR1_EL1
#define ID_ISAR2_EL1_get(v)                         MRS v,ID_ISAR2_EL1
#define ID_ISAR3_EL1_get(v)                         MRS v,ID_ISAR3_EL1
#define ID_ISAR4_EL1_get(v)                         MRS v,ID_ISAR4_EL1
#define ID_ISAR5_EL1_get(v)                         MRS v,ID_ISAR5_EL1
#define ID_MMFR0_EL1_get(v)                         MRS v,ID_MMFR0_EL1
#define ID_MMFR1_EL1_get(v)                         MRS v,ID_MMFR1_EL1
#define ID_MMFR2_EL1_get(v)                         MRS v,ID_MMFR2_EL1
#define ID_MMFR3_EL1_get(v)                         MRS v,ID_MMFR3_EL1
#define ID_MMFR4_EL1_get(v)                         MRS v,ID_MMFR4_EL1
#define ID_PFR0_EL1_get(v)                          MRS v,ID_PFR0_EL1
#define ID_PFR1_EL1_get(v)                          MRS v,ID_PFR1_EL1
#define MIDR_EL1_get(v)                             MRS v,MIDR_EL1
#define MPIDR_EL1_get(v)                            MRS v,MPIDR_EL1
#define MVFR0_EL1_get(v)                            MRS v,MVFR0_EL1
#define MVFR1_EL1_get(v)                            MRS v,MVFR1_EL1
#define MVFR2_EL1_get(v)                            MRS v,MVFR2_EL1
#define REVIDR_EL1_get(v)                           MRS v,REVIDR_EL1
#define VMPIDR_EL2_get(v)                           MRS v,VMPIDR_EL2
#define VMPIDR_EL2_set(v)                           MSR VMPIDR_EL2,v
#define VPIDR_EL2_get(v)                            MRS v,VPIDR_EL2
#define VPIDR_EL2_set(v)                            MSR VPIDR_EL2,v
#define PMCCFILTR_EL0_get(v)                        MRS v,PMCCFILTR_EL0
#define PMCCFILTR_EL0_set(v)                        MSR PMCCFILTR_EL0,v
#define PMCCNTR_EL0_get(v)                          MRS v,PMCCNTR_EL0
#define PMCCNTR_EL0_set(v)                          MSR PMCCNTR_EL0,v
#define PMCEID0_EL0_get(v)                          MRS v,PMCEID0_EL0
#define PMCEID1_EL0_get(v)                          MRS v,PMCEID1_EL0
#define PMCNTENCLR_EL0_get(v)                       MRS v,PMCNTENCLR_EL0
#define PMCNTENCLR_EL0_set(v)                       MSR PMCNTENCLR_EL0,v
#define PMCNTENSET_EL0_get(v)                       MRS v,PMCNTENSET_EL0
#define PMCNTENSET_EL0_set(v)                       MSR PMCNTENSET_EL0,v
#define PMCR_EL0_get(v)                             MRS v,PMCR_EL0
#define PMCR_EL0_set(v)                             MSR PMCR_EL0,v
#define PMEVCNTR_EL0_get(n,v)                      	MRS v,PMEVCNTR##n##_EL0
#define PMEVCNTR_EL0_set(n,v)                      	MSR PMEVCNTR##n##_EL0,v
#define PMEVTYPER_EL0_get(n,v)                     	MRS v,PMEVTYPER##n##_EL0
#define PMEVTYPER_EL0_set(n,v)                     	MSR PMEVTYPER##n##_EL0,v
#define PMINTENCLR_EL1_get(v)                       MRS v,PMINTENCLR_EL1
#define PMINTENCLR_EL1_set(v)                       MSR PMINTENCLR_EL1,v
#define PMINTENSET_EL1_get(v)                       MRS v,PMINTENSET_EL1
#define PMINTENSET_EL1_set(v)                       MSR PMINTENSET_EL1,v
#define PMOVSCLR_EL0_get(v)                         MRS v,PMOVSCLR_EL0
#define PMOVSCLR_EL0_set(v)                         MSR PMOVSCLR_EL0,v
#define PMOVSSET_EL0_get(v)                         MRS v,PMOVSSET_EL0
#define PMOVSSET_EL0_set(v)                         MSR PMOVSSET_EL0,v
#define PMSELR_EL0_get(v)                           MRS v,PMSELR_EL0
#define PMSELR_EL0_set(v)                           MSR PMSELR_EL0,v
#define PMSWINC_EL0_set(v)                          MSR PMSWINC_EL0,v
#define PMUSERENR_EL0_get(v)                        MRS v,PMUSERENR_EL0
#define PMUSERENR_EL0_set(v)                        MSR PMUSERENR_EL0,v
#define PMXEVCNTR_EL0_get(v)                        MRS v,PMXEVCNTR_EL0
#define PMXEVCNTR_EL0_set(v)                        MSR PMXEVCNTR_EL0,v
#define PMXEVTYPER_EL0_get(v)                       MRS v,PMXEVTYPER_EL0
#define PMXEVTYPER_EL0_set(v)                       MSR PMXEVTYPER_EL0,v
#define DBGAUTHSTATUS_EL1_get(v)                    MRS v,DBGAUTHSTATUS_EL1
#define DBGBCR_EL1_get(n,v)                        	MRS v,DBGBCR##n##_EL1
#define DBGBCR_EL1_set(n,v)                        	MSR DBGBCR##n##_EL1,v
#define DBGBVR_EL1_get(n,v)                        	MRS v,DBGBVR##n##_EL1
#define DBGBVR_EL1_set(n,v)                        	MSR DBGBVR##n##_EL1,v
#define DBGCLAIMCLR_EL1_get(v)                      MRS v,DBGCLAIMCLR_EL1
#define DBGCLAIMCLR_EL1_set(v)                      MSR DBGCLAIMCLR_EL1,v
#define DBGCLAIMSET_EL1_get(v)                      MRS v,DBGCLAIMSET_EL1
#define DBGCLAIMSET_EL1_set(v)                      MSR DBGCLAIMSET_EL1,v
#define DBGDTR_EL0_get(v)                           MRS v,DBGDTR_EL0
#define DBGDTR_EL0_set(v)                           MSR DBGDTR_EL0,v
#define DBGDTRRX_EL0_get(v)                         MRS v,DBGDTRRX_EL0
#define DBGDTRTX_EL0_set(v)                         MSR DBGDTRTX_EL0,v
#define DBGPRCR_EL1_get(v)                          MRS v,DBGPRCR_EL1
#define DBGPRCR_EL1_set(v)                          MSR DBGPRCR_EL1,v
#define DBGVCR32_EL2_get(v)                         MRS v,DBGVCR32_EL2
#define DBGVCR32_EL2_set(v)                         MSR DBGVCR32_EL2,v
#define DBGWCR_EL1_get(n,v)                        	MRS v,DBGWCR##n##_EL1
#define DBGWCR_EL1_set(n,v)                        	MSR DBGWCR##n##_EL1,v
#define DBGWVR_EL1_get(n,v)                        	MRS v,DBGWVR##n##_EL1
#define DBGWVR_EL1_set(n,v)                        	MSR DBGWVR##n##_EL1,v
#define DLR_EL0_get(v)                              MRS v,DLR_EL0
#define DLR_EL0_set(v)                              MSR DLR_EL0,v
#define DSPSR_EL0_get(v)                            MRS v,DSPSR_EL0
#define DSPSR_EL0_set(v)                            MSR DSPSR_EL0,v
#define MDCCINT_EL1_get(v)                          MRS v,MDCCINT_EL1
#define MDCCINT_EL1_set(v)                          MSR MDCCINT_EL1,v
#define MDCCSR_EL0_get(v)                           MRS v,MDCCSR_EL0
#define MDCR_EL2_get(v)                             MRS v,MDCR_EL2
#define MDCR_EL2_set(v)                             MSR MDCR_EL2,v
#define MDCR_EL3_get(v)                             MRS v,MDCR_EL3
#define MDCR_EL3_set(v)                             MSR MDCR_EL3,v
#define MDRAR_EL1_get(v)                            MRS v,MDRAR_EL1
#define MDSCR_EL1_get(v)                            MRS v,MDSCR_EL1
#define MDSCR_EL1_set(v)                            MSR MDSCR_EL1,v
#define OSDLR_EL1_get(v)                            MRS v,OSDLR_EL1
#define OSDLR_EL1_set(v)                            MSR OSDLR_EL1,v
#define OSDTRRX_EL1_get(v)                          MRS v,OSDTRRX_EL1
#define OSDTRRX_EL1_set(v)                          MSR OSDTRRX_EL1,v
#define OSDTRTX_EL1_get(v)                          MRS v,OSDTRTX_EL1
#define OSDTRTX_EL1_set(v)                          MSR OSDTRTX_EL1,v
#define OSECCR_EL1_get(v)                           MRS v,OSECCR_EL1
#define OSECCR_EL1_set(v)                           MSR OSECCR_EL1,v
#define OSLAR_EL1_set(v)                            MSR OSLAR_EL1,v
#define OSLSR_EL1_get(v)                            MRS v,OSLSR_EL1
#define SDER32_EL3_get(v)                           MRS v,SDER32_EL3
#define SDER32_EL3_set(v)                           MSR SDER32_EL3,v
#define CNTFRQ_EL0_get(v)                           MRS v,CNTFRQ_EL0
#define CNTFRQ_EL0_set(v)                           MSR CNTFRQ_EL0,v
#define CNTHCTL_EL2_get(v)                          MRS v,CNTHCTL_EL2
#define CNTHCTL_EL2_set(v)                          MSR CNTHCTL_EL2,v
#define CNTHP_CTL_EL2_get(v)                        MRS v,CNTHP_CTL_EL2
#define CNTHP_CTL_EL2_set(v)                        MSR CNTHP_CTL_EL2,v
#define CNTHP_CVAL_EL2_get(v)                       MRS v,CNTHP_CVAL_EL2
#define CNTHP_CVAL_EL2_set(v)                       MSR CNTHP_CVAL_EL2,v
#define CNTHP_TVAL_EL2_get(v)                       MRS v,CNTHP_TVAL_EL2
#define CNTHP_TVAL_EL2_set(v)                       MSR CNTHP_TVAL_EL2,v
#define CNTKCTL_EL1_get(v)                          MRS v,CNTKCTL_EL1
#define CNTKCTL_EL1_set(v)                          MSR CNTKCTL_EL1,v
#define CNTP_CTL_EL0_get(v)                         MRS v,CNTP_CTL_EL0
#define CNTP_CTL_EL0_set(v)                         MSR CNTP_CTL_EL0,v
#define CNTP_CVAL_EL0_get(v)                        MRS v,CNTP_CVAL_EL0
#define CNTP_CVAL_EL0_set(v)                        MSR CNTP_CVAL_EL0,v
#define CNTP_TVAL_EL0_get(v)                        MRS v,CNTP_TVAL_EL0
#define CNTP_TVAL_EL0_set(v)                        MSR CNTP_TVAL_EL0,v
#define CNTPCT_EL0_get(v)                           MRS v,CNTPCT_EL0
#define CNTPS_CTL_EL1_get(v)                        MRS v,CNTPS_CTL_EL1
#define CNTPS_CTL_EL1_set(v)                        MSR CNTPS_CTL_EL1,v
#define CNTPS_CVAL_EL1_get(v)                       MRS v,CNTPS_CVAL_EL1
#define CNTPS_CVAL_EL1_set(v)                       MSR CNTPS_CVAL_EL1,v
#define CNTPS_TVAL_EL1_get(v)                       MRS v,CNTPS_TVAL_EL1
#define CNTPS_TVAL_EL1_set(v)                       MSR CNTPS_TVAL_EL1,v
#define CNTV_CTL_EL0_get(v)                         MRS v,CNTV_CTL_EL0
#define CNTV_CTL_EL0_set(v)                         MSR CNTV_CTL_EL0,v
#define CNTV_CVAL_EL0_get(v)                        MRS v,CNTV_CVAL_EL0
#define CNTV_CVAL_EL0_set(v)                        MSR CNTV_CVAL_EL0,v
#define CNTV_TVAL_EL0_get(v)                        MRS v,CNTV_TVAL_EL0
#define CNTV_TVAL_EL0_set(v)                        MSR CNTV_TVAL_EL0,v
#define CNTVCT_EL0_get(v)                           MRS v,CNTVCT_EL0
#define CNTVOFF_EL2_get(v)                          MRS v,CNTVOFF_EL2
#define CNTVOFF_EL2_set(v)                          MSR CNTVOFF_EL2,v
#define DC_CISW_set(v)                              DC CISW, v
#define DC_CIVAC_set(v)                             DC CIVAC, v
#define DC_CSW_set(v)                               DC CSW, v
#define DC_CVAC_set(v)                              DC CVAC, v
#define DC_CVAU_set(v)                              DC CVAU, v
#define DC_ISW_set(v)                               DC ISW, v
#define DC_IVAC_set(v)                              DC IVAC, v
#define DC_ZVA_set(v)                               DC ZVA, v
#define IC_IALLU_set(v)                             IC IALLU, v
#define IC_IALLUIS_set(v)                           IC IALLUIS, v
#define AT_S12E0R_set(v)                            AT S12E0R, v
#define AT_S12E0W_set(v)                            AT S12E0W, v
#define AT_S12E1R_set(v)                            AT S12E1R, v
#define AT_S12E1W_set(v)                            AT S12E1W, v
#define AT_S1E0R_set(v)                             AT S1E0R, v
#define AT_S1E0W_set(v)                             AT S1E0W, v
#define AT_S1E1R_set(v)                             AT S1E1R, v
#define AT_S1E1W_set(v)                             AT S1E1W, v
#define AT_S1E2R_set(v)                             AT S1E2R, v
#define AT_S1E2W_set(v)                             AT S1E2W, v
#define AT_S1E3R_set(v)                             AT S1E3R, v
#define AT_S1E3W_set(v)                             AT S1E3W, v
#define TLBI_ASIDE1_set(v)                          TLBI ASIDE1, v
#define TLBI_ASIDE1IS_set(v)                        TLBI ASIDE1IS, v
#define TLBI_IPAS2E1_set(v)                         TLBI IPAS2E1, v
#define TLBI_IPAS2E1IS_set(v)                       TLBI IPAS2E1IS, v
#define TLBI_IPAS2LE1_set(v)                        TLBI IPAS2LE1, v
#define TLBI_IPAS2LE1IS_set(v)                      TLBI IPAS2LE1IS, v
#define TLBI_VAAE1_set(v)                           TLBI VAAE1, v
#define TLBI_VAAE1IS_set(v)                         TLBI VAAE1IS, v
#define TLBI_VAALE1_set(v)                          TLBI VAALE1, v
#define TLBI_VAALE1IS_set(v)                        TLBI VAALE1IS, v
#define TLBI_VAE1_set(v)                            TLBI VAE1, v
#define TLBI_VAE1IS_set(v)                          TLBI VAE1IS, v
#define TLBI_VAE2_set(v)                            TLBI VAE2, v
#define TLBI_VAE2IS_set(v)                          TLBI VAE2IS, v
#define TLBI_VAE3_set(v)                            TLBI VAE3, v
#define TLBI_VAE3IS_set(v)                          TLBI VAE3IS, v
#define TLBI_VALE1_set(v)                           TLBI VALE1, v
#define TLBI_VALE1IS_set(v)                         TLBI VALE1IS, v
#define TLBI_VALE2_set(v)                           TLBI VALE2, v
#define TLBI_VALE2IS_set(v)                         TLBI VALE2IS, v
#define TLBI_VALE3_set(v)                           TLBI VALE3, v
#define TLBI_VALE3IS_set(v)                         TLBI VALE3IS, v
#define ACTLR_EL1_get(v)                            MRS v,ACTLR_EL1
#define ACTLR_EL1_set(v)                            MSR ACTLR_EL1,v
#define ACTLR_EL2_get(v)                            MRS v,ACTLR_EL2
#define ACTLR_EL2_set(v)                            MSR ACTLR_EL2,v
#define ACTLR_EL3_get(v)                            MRS v,ACTLR_EL3
#define ACTLR_EL3_set(v)                            MSR ACTLR_EL3,v
#define AFSR0_EL1_get(v)                            MRS v,AFSR0_EL1
#define AFSR0_EL1_set(v)                            MSR AFSR0_EL1,v
#define AFSR0_EL2_get(v)                            MRS v,AFSR0_EL2
#define AFSR0_EL2_set(v)                            MSR AFSR0_EL2,v
#define AFSR0_EL3_get(v)                            MRS v,AFSR0_EL3
#define AFSR0_EL3_set(v)                            MSR AFSR0_EL3,v
#define AFSR1_EL1_get(v)                            MRS v,AFSR1_EL1
#define AFSR1_EL1_set(v)                            MSR AFSR1_EL1,v
#define AFSR1_EL2_get(v)                            MRS v,AFSR1_EL2
#define AFSR1_EL2_set(v)                            MSR AFSR1_EL2,v
#define AFSR1_EL3_get(v)                            MRS v,AFSR1_EL3
#define AFSR1_EL3_set(v)                            MSR AFSR1_EL3,v
#define CPACR_EL1_get(v)                            MRS v,CPACR_EL1
#define CPACR_EL1_set(v)                            MSR CPACR_EL1,v
#define CPTR_EL2_get(v)                             MRS v,CPTR_EL2
#define CPTR_EL2_set(v)                             MSR CPTR_EL2,v
#define CPTR_EL3_get(v)                             MRS v,CPTR_EL3
#define CPTR_EL3_set(v)                             MSR CPTR_EL3,v
#define CurrentEL_get(v)                            MRS v,CurrentEL
#define DAIF_get(v)                                 MRS v,DAIF
#define DAIF_set(v)                                 MSR DAIF,v
#define ESR_EL1_get(v)                              MRS v,ESR_EL1
#define ESR_EL1_set(v)                              MSR ESR_EL1,v
#define ESR_EL2_get(v)                              MRS v,ESR_EL2
#define ESR_EL2_set(v)                              MSR ESR_EL2,v
#define ESR_EL3_get(v)                              MRS v,ESR_EL3
#define ESR_EL3_set(v)                              MSR ESR_EL3,v
#define ESR_ELx_get(v)                              MRS v,ESR_ELx
#define ESR_ELx_set(v)                              MSR ESR_ELx,v
#define FAR_EL1_get(v)                              MRS v,FAR_EL1
#define FAR_EL1_set(v)                              MSR FAR_EL1,v
#define FAR_EL2_get(v)                              MRS v,FAR_EL2
#define FAR_EL2_set(v)                              MSR FAR_EL2,v
#define FAR_EL3_get(v)                              MRS v,FAR_EL3
#define FAR_EL3_set(v)                              MSR FAR_EL3,v
#define FPEXC32_EL2_get(v)                          MRS v,FPEXC32_EL2
#define FPEXC32_EL2_set(v)                          MSR FPEXC32_EL2,v
#define HACR_EL2_get(v)                             MRS v,HACR_EL2
#define HACR_EL2_set(v)                             MSR HACR_EL2,v
#define HCR_EL2_get(v)                              MRS v,HCR_EL2
#define HCR_EL2_set(v)                              MSR HCR_EL2,v
#define HPFAR_EL2_get(v)                            MRS v,HPFAR_EL2
#define HPFAR_EL2_set(v)                            MSR HPFAR_EL2,v
#define HSTR_EL2_get(v)                             MRS v,HSTR_EL2
#define HSTR_EL2_set(v)                             MSR HSTR_EL2,v
#define IFSR32_EL2_get(v)                           MRS v,IFSR32_EL2
#define IFSR32_EL2_set(v)                           MSR IFSR32_EL2,v
#define ISR_EL1_get(v)                              MRS v,ISR_EL1
#define NZCV_get(v)                                 MRS v,NZCV
#define NZCV_set(v)                                 MSR NZCV,v
#define PAR_EL1_get(v)                              MRS v,PAR_EL1
#define PAR_EL1_set(v)                              MSR PAR_EL1,v
#define RMR_EL1_get(v)                              MRS v,RMR_EL1
#define RMR_EL1_set(v)                              MSR RMR_EL1,v
#define RMR_EL2_get(v)                              MRS v,RMR_EL2
#define RMR_EL2_set(v)                              MSR RMR_EL2,v
#define RMR_EL3_get(v)                              MRS v,RMR_EL3
#define RMR_EL3_set(v)                              MSR RMR_EL3,v
#define RVBAR_EL1_get(v)                            MRS v,RVBAR_EL1
#define RVBAR_EL2_get(v)                            MRS v,RVBAR_EL2
#define RVBAR_EL3_get(v)                            MRS v,RVBAR_EL3
#define SCR_EL3_get(v)                              MRS v,SCR_EL3
#define SCR_EL3_set(v)                              MSR SCR_EL3,v
#define SCTLR_EL1_get(v)                            MRS v,SCTLR_EL1
#define SCTLR_EL1_set(v)                            MSR SCTLR_EL1,v
#define SCTLR_EL2_get(v)                            MRS v,SCTLR_EL2
#define SCTLR_EL2_set(v)                            MSR SCTLR_EL2,v
#define SCTLR_EL3_get(v)                            MRS v,SCTLR_EL3
#define SCTLR_EL3_set(v)                            MSR SCTLR_EL3,v
#define SPSel_get(v)                                MRS v,SPSel
#define SPSel_set(v)                                MSR SPSel,v
#define TPIDR_EL0_get(v)                            MRS v,TPIDR_EL0
#define TPIDR_EL0_set(v)                            MSR TPIDR_EL0,v
#define TPIDR_EL1_get(v)                            MRS v,TPIDR_EL1
#define TPIDR_EL1_set(v)                            MSR TPIDR_EL1,v
#define TPIDR_EL2_get(v)                            MRS v,TPIDR_EL2
#define TPIDR_EL2_set(v)                            MSR TPIDR_EL2,v
#define TPIDR_EL3_get(v)                            MRS v,TPIDR_EL3
#define TPIDR_EL3_set(v)                            MSR TPIDR_EL3,v
#define TPIDRRO_EL0_get(v)                          MRS v,TPIDRRO_EL0
#define TPIDRRO_EL0_set(v)                          MSR TPIDRRO_EL0,v
#define VBAR_EL1_get(v)                             MRS v,VBAR_EL1
#define VBAR_EL1_set(v)                             MSR VBAR_EL1,v
#define VBAR_EL2_get(v)                             MRS v,VBAR_EL2
#define VBAR_EL2_set(v)                             MSR VBAR_EL2,v
#define VBAR_EL3_get(v)                             MRS v,VBAR_EL3
#define VBAR_EL3_set(v)                             MSR VBAR_EL3,v
#define DAIFSet_seti(v)								MSR DAIFSet, #(v)
#define DAIFClr_seti(v)								MSR DAIFClr, #(v)
#define SPSel_seti(v)								MSR SPSel, #(v)

#else
#include <stdint.h>

#define DLR_EL0_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, DLR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DLR_EL0_set(v)                              __asm__ volatile ("MSR DLR_EL0, %0"::"r"(v):"memory")
#define DSPSR_EL0_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, DSPSR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DSPSR_EL0_set(v)                            __asm__ volatile ("MSR DSPSR_EL0, %0"::"r"(v):"memory")
#define ELR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ELR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ELR_EL1_set(v)                              __asm__ volatile ("MSR ELR_EL1, %0"::"r"(v):"memory")
#define ELR_EL2_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ELR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define ELR_EL2_set(v)                              __asm__ volatile ("MSR ELR_EL2, %0"::"r"(v):"memory")
#define ELR_EL3_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ELR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define ELR_EL3_set(v)                              __asm__ volatile ("MSR ELR_EL3, %0"::"r"(v):"memory")
#define FPCR_get()                                  ({uint64_t _reg_;__asm__ volatile ("MRS %0, FPCR":"=r"(_reg_)::"memory");_reg_;})
#define FPCR_set(v)                                 __asm__ volatile ("MSR FPCR, %0"::"r"(v):"memory")
#define FPSR_get()                                  ({uint64_t _reg_;__asm__ volatile ("MRS %0, FPSR":"=r"(_reg_)::"memory");_reg_;})
#define FPSR_set(v)                                 __asm__ volatile ("MSR FPSR, %0"::"r"(v):"memory")
#define SP_EL0_get()                                ({uint64_t _reg_;__asm__ volatile ("MRS %0, SP_EL0":"=r"(_reg_)::"memory");_reg_;})
#define SP_EL0_set(v)                               __asm__ volatile ("MSR SP_EL0, %0"::"r"(v):"memory")
#define SP_EL1_get()                                ({uint64_t _reg_;__asm__ volatile ("MRS %0, SP_EL1":"=r"(_reg_)::"memory");_reg_;})
#define SP_EL1_set(v)                               __asm__ volatile ("MSR SP_EL1, %0"::"r"(v):"memory")
#define SP_EL2_get()                                ({uint64_t _reg_;__asm__ volatile ("MRS %0, SP_EL2":"=r"(_reg_)::"memory");_reg_;})
#define SP_EL2_set(v)                               __asm__ volatile ("MSR SP_EL2, %0"::"r"(v):"memory")
#define SP_EL3_get()                                ({uint64_t _reg_;__asm__ volatile ("MRS %0, SP_EL3":"=r"(_reg_)::"memory");_reg_;})
#define SP_EL3_set(v)                               __asm__ volatile ("MSR SP_EL3, %0"::"r"(v):"memory")
#define SPSR_abt_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_abt":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_abt_set(v)                             __asm__ volatile ("MSR SPSR_abt, %0"::"r"(v):"memory")
#define SPSR_EL1_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_EL1_set(v)                             __asm__ volatile ("MSR SPSR_EL1, %0"::"r"(v):"memory")
#define SPSR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_EL2_set(v)                             __asm__ volatile ("MSR SPSR_EL2, %0"::"r"(v):"memory")
#define SPSR_EL3_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_EL3_set(v)                             __asm__ volatile ("MSR SPSR_EL3, %0"::"r"(v):"memory")
#define SPSR_fiq_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_fiq":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_fiq_set(v)                             __asm__ volatile ("MSR SPSR_fiq, %0"::"r"(v):"memory")
#define SPSR_irq_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_irq":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_irq_set(v)                             __asm__ volatile ("MSR SPSR_irq, %0"::"r"(v):"memory")
#define SPSR_und_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSR_und":"=r"(_reg_)::"memory");_reg_;})
#define SPSR_und_set(v)                             __asm__ volatile ("MSR SPSR_und, %0"::"r"(v):"memory")
#define AMAIR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AMAIR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define AMAIR_EL1_set(v)                            __asm__ volatile ("MSR AMAIR_EL1, %0"::"r"(v):"memory")
#define AMAIR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AMAIR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define AMAIR_EL2_set(v)                            __asm__ volatile ("MSR AMAIR_EL2, %0"::"r"(v):"memory")
#define AMAIR_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AMAIR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define AMAIR_EL3_set(v)                            __asm__ volatile ("MSR AMAIR_EL3, %0"::"r"(v):"memory")
#define CONTEXTIDR_EL1_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, CONTEXTIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CONTEXTIDR_EL1_set(v)                       __asm__ volatile ("MSR CONTEXTIDR_EL1, %0"::"r"(v):"memory")
#define DACR32_EL2_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, DACR32_EL2":"=r"(_reg_)::"memory");_reg_;})
#define DACR32_EL2_set(v)                           __asm__ volatile ("MSR DACR32_EL2, %0"::"r"(v):"memory")
#define MAIR_EL1_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, MAIR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MAIR_EL1_set(v)                             __asm__ volatile ("MSR MAIR_EL1, %0"::"r"(v):"memory")
#define MAIR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, MAIR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define MAIR_EL2_set(v)                             __asm__ volatile ("MSR MAIR_EL2, %0"::"r"(v):"memory")
#define MAIR_EL3_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, MAIR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define MAIR_EL3_set(v)                             __asm__ volatile ("MSR MAIR_EL3, %0"::"r"(v):"memory")
#define TCR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, TCR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define TCR_EL1_set(v)                              __asm__ volatile ("MSR TCR_EL1, %0"::"r"(v):"memory")
#define TCR_EL2_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, TCR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define TCR_EL2_set(v)                              __asm__ volatile ("MSR TCR_EL2, %0"::"r"(v):"memory")
#define TCR_EL3_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, TCR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define TCR_EL3_set(v)                              __asm__ volatile ("MSR TCR_EL3, %0"::"r"(v):"memory")
#define TTBR0_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TTBR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define TTBR0_EL1_set(v)                            __asm__ volatile ("MSR TTBR0_EL1, %0"::"r"(v):"memory")
#define TTBR0_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TTBR0_EL2":"=r"(_reg_)::"memory");_reg_;})
#define TTBR0_EL2_set(v)                            __asm__ volatile ("MSR TTBR0_EL2, %0"::"r"(v):"memory")
#define TTBR0_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TTBR0_EL3":"=r"(_reg_)::"memory");_reg_;})
#define TTBR0_EL3_set(v)                            __asm__ volatile ("MSR TTBR0_EL3, %0"::"r"(v):"memory")
#define TTBR1_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TTBR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define TTBR1_EL1_set(v)                            __asm__ volatile ("MSR TTBR1_EL1, %0"::"r"(v):"memory")
#define VTCR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, VTCR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define VTCR_EL2_set(v)                             __asm__ volatile ("MSR VTCR_EL2, %0"::"r"(v):"memory")
#define VTTBR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, VTTBR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define VTTBR_EL2_set(v)                            __asm__ volatile ("MSR VTTBR_EL2, %0"::"r"(v):"memory")
#define AIDR_EL1_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, AIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CCSIDR_EL1_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, CCSIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CLIDR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, CLIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CSSELR_EL1_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, CSSELR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CSSELR_EL1_set(v)                           __asm__ volatile ("MSR CSSELR_EL1, %0"::"r"(v):"memory")
#define CTR_EL0_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, CTR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DCZID_EL0_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, DCZID_EL0":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64AFR0_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64AFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64AFR1_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64AFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64DFR0_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64DFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64DFR1_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64DFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64ISAR0_EL1_get()                      ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64ISAR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64ISAR1_EL1_get()                      ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64ISAR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64MMFR0_EL1_get()                      ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64MMFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64MMFR1_EL1_get()                      ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64MMFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64PFR0_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64PFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AA64PFR1_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AA64PFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_AFR0_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_AFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_DFR0_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_DFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR0_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_ISAR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR1_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_ISAR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR2_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_ISAR2_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR3_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_ISAR3_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR4_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_ISAR4_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR5_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_ISAR5_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR0_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_MMFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR1_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_MMFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR2_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_MMFR2_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR3_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_MMFR3_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR4_EL1_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_MMFR4_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_PFR0_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_PFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ID_PFR1_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, ID_PFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MIDR_EL1_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, MIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MPIDR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, MPIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MVFR0_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, MVFR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MVFR1_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, MVFR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MVFR2_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, MVFR2_EL1":"=r"(_reg_)::"memory");_reg_;})
#define REVIDR_EL1_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, REVIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define VMPIDR_EL2_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, VMPIDR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define VMPIDR_EL2_set(v)                           __asm__ volatile ("MSR VMPIDR_EL2, %0"::"r"(v):"memory")
#define VPIDR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, VPIDR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define VPIDR_EL2_set(v)                            __asm__ volatile ("MSR VPIDR_EL2, %0"::"r"(v):"memory")
#define PMCCFILTR_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCCFILTR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCCFILTR_EL0_set(v)                        __asm__ volatile ("MSR PMCCFILTR_EL0, %0"::"r"(v):"memory")
#define PMCCNTR_EL0_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCCNTR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCCNTR_EL0_set(v)                          __asm__ volatile ("MSR PMCCNTR_EL0, %0"::"r"(v):"memory")
#define PMCEID0_EL0_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCEID0_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCEID1_EL0_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCEID1_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCNTENCLR_EL0_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCNTENCLR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCNTENCLR_EL0_set(v)                       __asm__ volatile ("MSR PMCNTENCLR_EL0, %0"::"r"(v):"memory")
#define PMCNTENSET_EL0_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCNTENSET_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCNTENSET_EL0_set(v)                       __asm__ volatile ("MSR PMCNTENSET_EL0, %0"::"r"(v):"memory")
#define PMCR_EL0_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMCR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMCR_EL0_set(v)                             __asm__ volatile ("MSR PMCR_EL0, %0"::"r"(v):"memory")
#define PMEVCNTR_EL0_get(n)                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMEVCNTR" #n "_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMEVCNTR_EL0_set(n,v)                      __asm__ volatile ("MSR PMEVCNTR" #n "_EL0, %0"::"r"(v):"memory")
#define PMEVTYPER_EL0_get(n)                      ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMEVTYPER" #n "_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMEVTYPER_EL0_set(n,v)                     __asm__ volatile ("MSR PMEVTYPER" #n "_EL0, %0"::"r"(v):"memory")
#define PMINTENCLR_EL1_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMINTENCLR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define PMINTENCLR_EL1_set(v)                       __asm__ volatile ("MSR PMINTENCLR_EL1, %0"::"r"(v):"memory")
#define PMINTENSET_EL1_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMINTENSET_EL1":"=r"(_reg_)::"memory");_reg_;})
#define PMINTENSET_EL1_set(v)                       __asm__ volatile ("MSR PMINTENSET_EL1, %0"::"r"(v):"memory")
#define PMOVSCLR_EL0_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMOVSCLR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMOVSCLR_EL0_set(v)                         __asm__ volatile ("MSR PMOVSCLR_EL0, %0"::"r"(v):"memory")
#define PMOVSSET_EL0_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMOVSSET_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMOVSSET_EL0_set(v)                         __asm__ volatile ("MSR PMOVSSET_EL0, %0"::"r"(v):"memory")
#define PMSELR_EL0_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMSELR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMSELR_EL0_set(v)                           __asm__ volatile ("MSR PMSELR_EL0, %0"::"r"(v):"memory")
#define PMSWINC_EL0_set(v)                          __asm__ volatile ("MSR PMSWINC_EL0, %0"::"r"(v):"memory")
#define PMUSERENR_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMUSERENR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMUSERENR_EL0_set(v)                        __asm__ volatile ("MSR PMUSERENR_EL0, %0"::"r"(v):"memory")
#define PMXEVCNTR_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMXEVCNTR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMXEVCNTR_EL0_set(v)                        __asm__ volatile ("MSR PMXEVCNTR_EL0, %0"::"r"(v):"memory")
#define PMXEVTYPER_EL0_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, PMXEVTYPER_EL0":"=r"(_reg_)::"memory");_reg_;})
#define PMXEVTYPER_EL0_set(v)                       __asm__ volatile ("MSR PMXEVTYPER_EL0, %0"::"r"(v):"memory")
#define DBGAUTHSTATUS_EL1_get()                     ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGAUTHSTATUS_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGBCR_EL1_get(n)                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGBCR" #n "_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGBCR_EL1_set(n,v)                        __asm__ volatile ("MSR DBGBCR" #n "_EL1, %0"::"r"(v):"memory")
#define DBGBVR_EL1_get(n)                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGBVR" #n "_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGBVR_EL1_set(n,v)                        __asm__ volatile ("MSR DBGBVR" #n "_EL1, %0"::"r"(v):"memory")
#define DBGCLAIMCLR_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGCLAIMCLR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGCLAIMCLR_EL1_set(v)                      __asm__ volatile ("MSR DBGCLAIMCLR_EL1, %0"::"r"(v):"memory")
#define DBGCLAIMSET_EL1_get()                       ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGCLAIMSET_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGCLAIMSET_EL1_set(v)                      __asm__ volatile ("MSR DBGCLAIMSET_EL1, %0"::"r"(v):"memory")
#define DBGDTR_EL0_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGDTR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DBGDTR_EL0_set(v)                           __asm__ volatile ("MSR DBGDTR_EL0, %0"::"r"(v):"memory")
#define DBGDTRRX_EL0_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGDTRRX_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DBGDTRTX_EL0_set(v)                         __asm__ volatile ("MSR DBGDTRTX_EL0, %0"::"r"(v):"memory")
#define DBGPRCR_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGPRCR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGPRCR_EL1_set(v)                          __asm__ volatile ("MSR DBGPRCR_EL1, %0"::"r"(v):"memory")
#define DBGVCR32_EL2_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGVCR32_EL2":"=r"(_reg_)::"memory");_reg_;})
#define DBGVCR32_EL2_set(v)                         __asm__ volatile ("MSR DBGVCR32_EL2, %0"::"r"(v):"memory")
#define DBGWCR_EL1_get(n)                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGWCR" #n "_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGWCR_EL1_set(n,v)                        __asm__ volatile ("MSR DBGWCR" #n "_EL1, %0"::"r"(v):"memory")
#define DBGWVR_EL1_get(n)                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, DBGWVR" #n "_EL1":"=r"(_reg_)::"memory");_reg_;})
#define DBGWVR_EL1_set(n,v)                        __asm__ volatile ("MSR DBGWVR" #n "_EL1, %0"::"r"(v):"memory")
#define DLR_EL0_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, DLR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DLR_EL0_set(v)                              __asm__ volatile ("MSR DLR_EL0, %0"::"r"(v):"memory")
#define DSPSR_EL0_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, DSPSR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define DSPSR_EL0_set(v)                            __asm__ volatile ("MSR DSPSR_EL0, %0"::"r"(v):"memory")
#define MDCCINT_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, MDCCINT_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MDCCINT_EL1_set(v)                          __asm__ volatile ("MSR MDCCINT_EL1, %0"::"r"(v):"memory")
#define MDCCSR_EL0_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, MDCCSR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define MDCR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, MDCR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define MDCR_EL2_set(v)                             __asm__ volatile ("MSR MDCR_EL2, %0"::"r"(v):"memory")
#define MDCR_EL3_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, MDCR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define MDCR_EL3_set(v)                             __asm__ volatile ("MSR MDCR_EL3, %0"::"r"(v):"memory")
#define MDRAR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, MDRAR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MDSCR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, MDSCR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define MDSCR_EL1_set(v)                            __asm__ volatile ("MSR MDSCR_EL1, %0"::"r"(v):"memory")
#define OSDLR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, OSDLR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define OSDLR_EL1_set(v)                            __asm__ volatile ("MSR OSDLR_EL1, %0"::"r"(v):"memory")
#define OSDTRRX_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, OSDTRRX_EL1":"=r"(_reg_)::"memory");_reg_;})
#define OSDTRRX_EL1_set(v)                          __asm__ volatile ("MSR OSDTRRX_EL1, %0"::"r"(v):"memory")
#define OSDTRTX_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, OSDTRTX_EL1":"=r"(_reg_)::"memory");_reg_;})
#define OSDTRTX_EL1_set(v)                          __asm__ volatile ("MSR OSDTRTX_EL1, %0"::"r"(v):"memory")
#define OSECCR_EL1_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, OSECCR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define OSECCR_EL1_set(v)                           __asm__ volatile ("MSR OSECCR_EL1, %0"::"r"(v):"memory")
#define OSLAR_EL1_set(v)                            __asm__ volatile ("MSR OSLAR_EL1, %0"::"r"(v):"memory")
#define OSLSR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, OSLSR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define SDER32_EL3_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, SDER32_EL3":"=r"(_reg_)::"memory");_reg_;})
#define SDER32_EL3_set(v)                           __asm__ volatile ("MSR SDER32_EL3, %0"::"r"(v):"memory")
#define CNTFRQ_EL0_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTFRQ_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTFRQ_EL0_set(v)                           __asm__ volatile ("MSR CNTFRQ_EL0, %0"::"r"(v):"memory")
#define CNTHCTL_EL2_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTHCTL_EL2":"=r"(_reg_)::"memory");_reg_;})
#define CNTHCTL_EL2_set(v)                          __asm__ volatile ("MSR CNTHCTL_EL2, %0"::"r"(v):"memory")
#define CNTHP_CTL_EL2_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTHP_CTL_EL2":"=r"(_reg_)::"memory");_reg_;})
#define CNTHP_CTL_EL2_set(v)                        __asm__ volatile ("MSR CNTHP_CTL_EL2, %0"::"r"(v):"memory")
#define CNTHP_CVAL_EL2_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTHP_CVAL_EL2":"=r"(_reg_)::"memory");_reg_;})
#define CNTHP_CVAL_EL2_set(v)                       __asm__ volatile ("MSR CNTHP_CVAL_EL2, %0"::"r"(v):"memory")
#define CNTHP_TVAL_EL2_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTHP_TVAL_EL2":"=r"(_reg_)::"memory");_reg_;})
#define CNTHP_TVAL_EL2_set(v)                       __asm__ volatile ("MSR CNTHP_TVAL_EL2, %0"::"r"(v):"memory")
#define CNTKCTL_EL1_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTKCTL_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CNTKCTL_EL1_set(v)                          __asm__ volatile ("MSR CNTKCTL_EL1, %0"::"r"(v):"memory")
#define CNTP_CTL_EL0_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTP_CTL_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTP_CTL_EL0_set(v)                         __asm__ volatile ("MSR CNTP_CTL_EL0, %0"::"r"(v):"memory")
#define CNTP_CVAL_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTP_CVAL_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTP_CVAL_EL0_set(v)                        __asm__ volatile ("MSR CNTP_CVAL_EL0, %0"::"r"(v):"memory")
#define CNTP_TVAL_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTP_TVAL_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTP_TVAL_EL0_set(v)                        __asm__ volatile ("MSR CNTP_TVAL_EL0, %0"::"r"(v):"memory")
#define CNTPCT_EL0_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTPCT_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTPS_CTL_EL1_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTPS_CTL_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CNTPS_CTL_EL1_set(v)                        __asm__ volatile ("MSR CNTPS_CTL_EL1, %0"::"r"(v):"memory")
#define CNTPS_CVAL_EL1_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTPS_CVAL_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CNTPS_CVAL_EL1_set(v)                       __asm__ volatile ("MSR CNTPS_CVAL_EL1, %0"::"r"(v):"memory")
#define CNTPS_TVAL_EL1_get()                        ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTPS_TVAL_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CNTPS_TVAL_EL1_set(v)                       __asm__ volatile ("MSR CNTPS_TVAL_EL1, %0"::"r"(v):"memory")
#define CNTV_CTL_EL0_get()                          ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTV_CTL_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTV_CTL_EL0_set(v)                         __asm__ volatile ("MSR CNTV_CTL_EL0, %0"::"r"(v):"memory")
#define CNTV_CVAL_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTV_CVAL_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTV_CVAL_EL0_set(v)                        __asm__ volatile ("MSR CNTV_CVAL_EL0, %0"::"r"(v):"memory")
#define CNTV_TVAL_EL0_get()                         ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTV_TVAL_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTV_TVAL_EL0_set(v)                        __asm__ volatile ("MSR CNTV_TVAL_EL0, %0"::"r"(v):"memory")
#define CNTVCT_EL0_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTVCT_EL0":"=r"(_reg_)::"memory");_reg_;})
#define CNTVOFF_EL2_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, CNTVOFF_EL2":"=r"(_reg_)::"memory");_reg_;})
#define CNTVOFF_EL2_set(v)                          __asm__ volatile ("MSR CNTVOFF_EL2, %0"::"r"(v):"memory")
#define DC_CISW_set(v)                              __asm__ volatile ("DC CISW, %0"::"r"(v):"memory")
#define DC_CIVAC_set(v)                             __asm__ volatile ("DC CIVAC, %0"::"r"(v):"memory")
#define DC_CSW_set(v)                               __asm__ volatile ("DC CSW, %0"::"r"(v):"memory")
#define DC_CVAC_set(v)                              __asm__ volatile ("DC CVAC, %0"::"r"(v):"memory")
#define DC_CVAU_set(v)                              __asm__ volatile ("DC CVAU, %0"::"r"(v):"memory")
#define DC_ISW_set(v)                               __asm__ volatile ("DC ISW, %0"::"r"(v):"memory")
#define DC_IVAC_set(v)                              __asm__ volatile ("DC IVAC, %0"::"r"(v):"memory")
#define DC_ZVA_set(v)                               __asm__ volatile ("DC ZVA, %0"::"r"(v):"memory")
#define IC_IALLU_set(v)                             __asm__ volatile ("IC IALLU, %0"::"r"(v):"memory")
#define IC_IALLUIS_set(v)                           __asm__ volatile ("IC IALLUIS, %0"::"r"(v):"memory")
#define AT_S12E0R_set(v)                            __asm__ volatile ("AT S12E0R, %0"::"r"(v):"memory")
#define AT_S12E0W_set(v)                            __asm__ volatile ("AT S12E0W, %0"::"r"(v):"memory")
#define AT_S12E1R_set(v)                            __asm__ volatile ("AT S12E1R, %0"::"r"(v):"memory")
#define AT_S12E1W_set(v)                            __asm__ volatile ("AT S12E1W, %0"::"r"(v):"memory")
#define AT_S1E0R_set(v)                             __asm__ volatile ("AT S1E0R, %0"::"r"(v):"memory")
#define AT_S1E0W_set(v)                             __asm__ volatile ("AT S1E0W, %0"::"r"(v):"memory")
#define AT_S1E1R_set(v)                             __asm__ volatile ("AT S1E1R, %0"::"r"(v):"memory")
#define AT_S1E1W_set(v)                             __asm__ volatile ("AT S1E1W, %0"::"r"(v):"memory")
#define AT_S1E2R_set(v)                             __asm__ volatile ("AT S1E2R, %0"::"r"(v):"memory")
#define AT_S1E2W_set(v)                             __asm__ volatile ("AT S1E2W, %0"::"r"(v):"memory")
#define AT_S1E3R_set(v)                             __asm__ volatile ("AT S1E3R, %0"::"r"(v):"memory")
#define AT_S1E3W_set(v)                             __asm__ volatile ("AT S1E3W, %0"::"r"(v):"memory")
#define TLBI_ASIDE1_set(v)                          __asm__ volatile ("TLBI ASIDE1, %0"::"r"(v):"memory")
#define TLBI_ASIDE1IS_set(v)                        __asm__ volatile ("TLBI ASIDE1IS, %0"::"r"(v):"memory")
#define TLBI_IPAS2E1_set(v)                         __asm__ volatile ("TLBI IPAS2E1, %0"::"r"(v):"memory")
#define TLBI_IPAS2E1IS_set(v)                       __asm__ volatile ("TLBI IPAS2E1IS, %0"::"r"(v):"memory")
#define TLBI_IPAS2LE1_set(v)                        __asm__ volatile ("TLBI IPAS2LE1, %0"::"r"(v):"memory")
#define TLBI_IPAS2LE1IS_set(v)                      __asm__ volatile ("TLBI IPAS2LE1IS, %0"::"r"(v):"memory")
#define TLBI_VAAE1_set(v)                           __asm__ volatile ("TLBI VAAE1, %0"::"r"(v):"memory")
#define TLBI_VAAE1IS_set(v)                         __asm__ volatile ("TLBI VAAE1IS, %0"::"r"(v):"memory")
#define TLBI_VAALE1_set(v)                          __asm__ volatile ("TLBI VAALE1, %0"::"r"(v):"memory")
#define TLBI_VAALE1IS_set(v)                        __asm__ volatile ("TLBI VAALE1IS, %0"::"r"(v):"memory")
#define TLBI_VAE1_set(v)                            __asm__ volatile ("TLBI VAE1, %0"::"r"(v):"memory")
#define TLBI_VAE1IS_set(v)                          __asm__ volatile ("TLBI VAE1IS, %0"::"r"(v):"memory")
#define TLBI_VAE2_set(v)                            __asm__ volatile ("TLBI VAE2, %0"::"r"(v):"memory")
#define TLBI_VAE2IS_set(v)                          __asm__ volatile ("TLBI VAE2IS, %0"::"r"(v):"memory")
#define TLBI_VAE3_set(v)                            __asm__ volatile ("TLBI VAE3, %0"::"r"(v):"memory")
#define TLBI_VAE3IS_set(v)                          __asm__ volatile ("TLBI VAE3IS, %0"::"r"(v):"memory")
#define TLBI_VALE1_set(v)                           __asm__ volatile ("TLBI VALE1, %0"::"r"(v):"memory")
#define TLBI_VALE1IS_set(v)                         __asm__ volatile ("TLBI VALE1IS, %0"::"r"(v):"memory")
#define TLBI_VALE2_set(v)                           __asm__ volatile ("TLBI VALE2, %0"::"r"(v):"memory")
#define TLBI_VALE2IS_set(v)                         __asm__ volatile ("TLBI VALE2IS, %0"::"r"(v):"memory")
#define TLBI_VALE3_set(v)                           __asm__ volatile ("TLBI VALE3, %0"::"r"(v):"memory")
#define TLBI_VALE3IS_set(v)                         __asm__ volatile ("TLBI VALE3IS, %0"::"r"(v):"memory")
#define ACTLR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, ACTLR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ACTLR_EL1_set(v)                            __asm__ volatile ("MSR ACTLR_EL1, %0"::"r"(v):"memory")
#define ACTLR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, ACTLR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define ACTLR_EL2_set(v)                            __asm__ volatile ("MSR ACTLR_EL2, %0"::"r"(v):"memory")
#define ACTLR_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, ACTLR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define ACTLR_EL3_set(v)                            __asm__ volatile ("MSR ACTLR_EL3, %0"::"r"(v):"memory")
#define AFSR0_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AFSR0_EL1":"=r"(_reg_)::"memory");_reg_;})
#define AFSR0_EL1_set(v)                            __asm__ volatile ("MSR AFSR0_EL1, %0"::"r"(v):"memory")
#define AFSR0_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AFSR0_EL2":"=r"(_reg_)::"memory");_reg_;})
#define AFSR0_EL2_set(v)                            __asm__ volatile ("MSR AFSR0_EL2, %0"::"r"(v):"memory")
#define AFSR0_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AFSR0_EL3":"=r"(_reg_)::"memory");_reg_;})
#define AFSR0_EL3_set(v)                            __asm__ volatile ("MSR AFSR0_EL3, %0"::"r"(v):"memory")
#define AFSR1_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AFSR1_EL1":"=r"(_reg_)::"memory");_reg_;})
#define AFSR1_EL1_set(v)                            __asm__ volatile ("MSR AFSR1_EL1, %0"::"r"(v):"memory")
#define AFSR1_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AFSR1_EL2":"=r"(_reg_)::"memory");_reg_;})
#define AFSR1_EL2_set(v)                            __asm__ volatile ("MSR AFSR1_EL2, %0"::"r"(v):"memory")
#define AFSR1_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, AFSR1_EL3":"=r"(_reg_)::"memory");_reg_;})
#define AFSR1_EL3_set(v)                            __asm__ volatile ("MSR AFSR1_EL3, %0"::"r"(v):"memory")
#define CPACR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, CPACR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define CPACR_EL1_set(v)                            __asm__ volatile ("MSR CPACR_EL1, %0"::"r"(v):"memory")
#define CPTR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, CPTR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define CPTR_EL2_set(v)                             __asm__ volatile ("MSR CPTR_EL2, %0"::"r"(v):"memory")
#define CPTR_EL3_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, CPTR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define CPTR_EL3_set(v)                             __asm__ volatile ("MSR CPTR_EL3, %0"::"r"(v):"memory")
#define CurrentEL_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, CurrentEL":"=r"(_reg_)::"memory");_reg_;})
#define DAIF_get()                                  ({uint64_t _reg_;__asm__ volatile ("MRS %0, DAIF":"=r"(_reg_)::"memory");_reg_;})
#define DAIF_set(v)                                 __asm__ volatile ("MSR DAIF, %0"::"r"(v):"memory")
#define ESR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ESR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define ESR_EL1_set(v)                              __asm__ volatile ("MSR ESR_EL1, %0"::"r"(v):"memory")
#define ESR_EL2_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ESR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define ESR_EL2_set(v)                              __asm__ volatile ("MSR ESR_EL2, %0"::"r"(v):"memory")
#define ESR_EL3_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ESR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define ESR_EL3_set(v)                              __asm__ volatile ("MSR ESR_EL3, %0"::"r"(v):"memory")
#define ESR_ELx_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ESR_ELx":"=r"(_reg_)::"memory");_reg_;})
#define ESR_ELx_set(v)                              __asm__ volatile ("MSR ESR_ELx, %0"::"r"(v):"memory")
#define FAR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, FAR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define FAR_EL1_set(v)                              __asm__ volatile ("MSR FAR_EL1, %0"::"r"(v):"memory")
#define FAR_EL2_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, FAR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define FAR_EL2_set(v)                              __asm__ volatile ("MSR FAR_EL2, %0"::"r"(v):"memory")
#define FAR_EL3_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, FAR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define FAR_EL3_set(v)                              __asm__ volatile ("MSR FAR_EL3, %0"::"r"(v):"memory")
#define FPEXC32_EL2_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, FPEXC32_EL2":"=r"(_reg_)::"memory");_reg_;})
#define FPEXC32_EL2_set(v)                          __asm__ volatile ("MSR FPEXC32_EL2, %0"::"r"(v):"memory")
#define HACR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, HACR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define HACR_EL2_set(v)                             __asm__ volatile ("MSR HACR_EL2, %0"::"r"(v):"memory")
#define HCR_EL2_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, HCR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define HCR_EL2_set(v)                              __asm__ volatile ("MSR HCR_EL2, %0"::"r"(v):"memory")
#define HPFAR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, HPFAR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define HPFAR_EL2_set(v)                            __asm__ volatile ("MSR HPFAR_EL2, %0"::"r"(v):"memory")
#define HSTR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, HSTR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define HSTR_EL2_set(v)                             __asm__ volatile ("MSR HSTR_EL2, %0"::"r"(v):"memory")
#define IFSR32_EL2_get()                            ({uint64_t _reg_;__asm__ volatile ("MRS %0, IFSR32_EL2":"=r"(_reg_)::"memory");_reg_;})
#define IFSR32_EL2_set(v)                           __asm__ volatile ("MSR IFSR32_EL2, %0"::"r"(v):"memory")
#define ISR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, ISR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define NZCV_get()                                  ({uint64_t _reg_;__asm__ volatile ("MRS %0, NZCV":"=r"(_reg_)::"memory");_reg_;})
#define NZCV_set(v)                                 __asm__ volatile ("MSR NZCV, %0"::"r"(v):"memory")
#define PAR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, PAR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define PAR_EL1_set(v)                              __asm__ volatile ("MSR PAR_EL1, %0"::"r"(v):"memory")
#define RMR_EL1_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, RMR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define RMR_EL1_set(v)                              __asm__ volatile ("MSR RMR_EL1, %0"::"r"(v):"memory")
#define RMR_EL2_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, RMR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define RMR_EL2_set(v)                              __asm__ volatile ("MSR RMR_EL2, %0"::"r"(v):"memory")
#define RMR_EL3_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, RMR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define RMR_EL3_set(v)                              __asm__ volatile ("MSR RMR_EL3, %0"::"r"(v):"memory")
#define RVBAR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, RVBAR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define RVBAR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, RVBAR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define RVBAR_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, RVBAR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define SCR_EL3_get()                               ({uint64_t _reg_;__asm__ volatile ("MRS %0, SCR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define SCR_EL3_set(v)                              __asm__ volatile ("MSR SCR_EL3, %0"::"r"(v):"memory")
#define SCTLR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, SCTLR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define SCTLR_EL1_set(v)                            __asm__ volatile ("MSR SCTLR_EL1, %0"::"r"(v):"memory")
#define SCTLR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, SCTLR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define SCTLR_EL2_set(v)                            __asm__ volatile ("MSR SCTLR_EL2, %0"::"r"(v):"memory")
#define SCTLR_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, SCTLR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define SCTLR_EL3_set(v)                            __asm__ volatile ("MSR SCTLR_EL3, %0"::"r"(v):"memory")
#define SPSel_get()                                 ({uint64_t _reg_;__asm__ volatile ("MRS %0, SPSel":"=r"(_reg_)::"memory");_reg_;})
#define SPSel_set(v)                                __asm__ volatile ("MSR SPSel, %0"::"r"(v):"memory")
#define TPIDR_EL0_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TPIDR_EL0":"=r"(_reg_)::"memory");_reg_;})
#define TPIDR_EL0_set(v)                            __asm__ volatile ("MSR TPIDR_EL0, %0"::"r"(v):"memory")
#define TPIDR_EL1_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TPIDR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define TPIDR_EL1_set(v)                            __asm__ volatile ("MSR TPIDR_EL1, %0"::"r"(v):"memory")
#define TPIDR_EL2_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TPIDR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define TPIDR_EL2_set(v)                            __asm__ volatile ("MSR TPIDR_EL2, %0"::"r"(v):"memory")
#define TPIDR_EL3_get()                             ({uint64_t _reg_;__asm__ volatile ("MRS %0, TPIDR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define TPIDR_EL3_set(v)                            __asm__ volatile ("MSR TPIDR_EL3, %0"::"r"(v):"memory")
#define TPIDRRO_EL0_get()                           ({uint64_t _reg_;__asm__ volatile ("MRS %0, TPIDRRO_EL0":"=r"(_reg_)::"memory");_reg_;})
#define TPIDRRO_EL0_set(v)                          __asm__ volatile ("MSR TPIDRRO_EL0, %0"::"r"(v):"memory")
#define VBAR_EL1_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, VBAR_EL1":"=r"(_reg_)::"memory");_reg_;})
#define VBAR_EL1_set(v)                             __asm__ volatile ("MSR VBAR_EL1, %0"::"r"(v):"memory")
#define VBAR_EL2_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, VBAR_EL2":"=r"(_reg_)::"memory");_reg_;})
#define VBAR_EL2_set(v)                             __asm__ volatile ("MSR VBAR_EL2, %0"::"r"(v):"memory")
#define VBAR_EL3_get()                              ({uint64_t _reg_;__asm__ volatile ("MRS %0, VBAR_EL3":"=r"(_reg_)::"memory");_reg_;})
#define VBAR_EL3_set(v)                             __asm__ volatile ("MSR VBAR_EL3, %0"::"r"(v):"memory")
#define DAIFSet_seti(v)								__asm__ volatile ("MSR DAIFSet, #" #v:::"memory")
#define DAIFClr_seti(v)								__asm__ volatile ("MSR DAIFClr, #" #v:::"memory")
#define SPSel_seti(v)								__asm__ volatile ("MSR SPSel, #" #v:::"memory")

#endif

#endif /* _ARMV8REG_H_ */
