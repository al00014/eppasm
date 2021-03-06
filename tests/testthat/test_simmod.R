context("test model simulation")

pjnz <- system.file("extdata/testpjnz", "Botswana2018.PJNZ", package="eppasm")
bw <- prepare_spec_fit(pjnz, proj.end=2022.5)

bw_fp <- attr(bw$Urban, "specfp")

bw_fp <- prepare_rhybrid(bw_fp)
bw_fp$ancsitedata <- TRUE
bw_fp$ancrt <- "census"
bw_fp$ancrtsite.beta <- 0
bw_fp$logitiota <- TRUE

bw_theta <- c(-0.407503322169364, -2.76794181367538, -1.26018073624346, 1995.96447776502,
              -0.00307437171215574, 0.0114118307148102, 0.00760958379603691, 0.02,
              2.24103194827232, -0.0792123921862689, -5.01917961803606, 0.359444135205712,
              -6.10051517060137)

param <- fnCreateParam(bw_theta, bw_fp)
bw_fp <- update(bw_fp, list=param)

bw_prev_mod <- c(0.00045, 0.00080, 0.0014, 0.00245, 0.00424, 0.00725, 0.01214,
                 0.01985, 0.03147, 0.04804, 0.07013, 0.0975, 0.12857, 0.16083,
                 0.19143, 0.21797, 0.23908, 0.25419, 0.26363, 0.26817, 0.26871,
                 0.26618, 0.26129, 0.2551, 0.24887, 0.24365, 0.23928, 0.23545,
                 0.23186, 0.22866, 0.22569, 0.22277, 0.21931, 0.21555, 0.21147,
                 0.2069, 0.20155, 0.19559, 0.1893, 0.1829, 0.1763, 0.16953, 0.16266)

test_that("model simulation returns correct prevalence", {
  expect_equal(round(prev(simmod(bw_fp))[11:53], 5), bw_prev_mod)
  expect_equal(round(prev(simmod(bw_fp, VERSION="R"))[11:53], 5), bw_prev_mod)
})


pjnz <- system.file("extdata/testpjnz", "Mozambique_Maputo_Cidade2018.PJNZ", package="eppasm")
mpm <- prepare_spec_fit(pjnz, proj.end=2021.5)

mp_fp <- attr(mpm[[1]], "specfp")

mp_fp <- prepare_rhybrid(mp_fp)
mp_fp$ancsitedata <- TRUE
mp_fp$ancrt <- "census"
mp_fp$ancrtsite.beta <- 0
mp_fp$logitiota <- TRUE

theta <- c(-0.407503322169364, -2.76794181367538, -1.26018073624346, 1995.96447776502,
           -0.00307437171215574, 0.0114118307148102, 0.00760958379603691, 0.02,
           2.24103194827232, -0.0792123921862689, -5.01917961803606, 0.359444135205712,
           -6.10051517060137)

param <- fnCreateParam(theta, mp_fp)
mp_fp <- update(mp_fp, list=param)

mp_prev_mod <- c(0.00049, 0.00087, 0.00154, 0.00271, 0.00468, 0.00792, 0.01299,
                 0.0207, 0.03198, 0.04796, 0.0696, 0.09711, 0.12905, 0.16256,
                 0.19448, 0.22199, 0.24389, 0.2597, 0.26977, 0.27482, 0.27557,
                 0.27277, 0.26702, 0.25891, 0.24924, 0.23878, 0.22871, 0.2203,
                 0.21375, 0.20835, 0.20326, 0.19834, 0.19338, 0.18835, 0.18232,
                 0.1762, 0.16976, 0.16322, 0.15678, 0.15029, 0.14384, 0.13677)

test_that("Mozambique Maputo Cidade returns correct prevalence", {
  expect_equal(round(prev(simmod(mp_fp))[11:52], 5), mp_prev_mod)
  expect_equal(round(prev(simmod(mp_fp, VERSION="R"))[11:52], 5), mp_prev_mod)
})

nl_fp <- prepare_directincid(system.file("extdata/testpjnz", "Netherlands2017.PJNZ", package="eppasm"))

hivpop_mod <- c(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38.742753, 277.452295, 832.078741,
                1724.498848, 2882.220122, 4143.634016, 5317.562667, 6266.262444,
                6921.247119, 7283.072597, 7424.889773, 7454.23537, 7466.1148,
                7525.781945, 7670.608489, 7903.543258, 8261.495471, 8972.10866,
                9864.808687, 10798.179973, 11737.594922, 12673.684559, 13599.710245,
                14511.932267, 15409.679039, 16291.691362, 17156.349592, 17999.580911,
                18819.704793, 19634.361175, 20445.376619, 21216.911195, 21905.607905,
                22466.527243, 22860.568652, 23047.112527, 23252.090943, 23474.675131,
                23673.20584, 23836.395946, 23964.524599, 24062.184597)

test_that("Netherlands returns correct HIV population size", {
  expect_equal(round(colSums(simmod(nl_fp)[,,2,],,2), 6), hivpop_mod)
  expect_equal(round(colSums(simmod(nl_fp, "R")[,,2,],,2), 6), hivpop_mod)
})



test_that("hivpop1 and artpop1 align with hivpop and artpop", {
  mod <- simmod(bw_fp)
  hp1 <- hivpop_singleage(mod, bw_fp$ss)
  expect_equal(colSums(hp1$hivpop1,,2), colSums(attr(mod, "hivpop"),,2))
  expect_equal(colSums(hp1$artpop1,,3), colSums(attr(mod, "artpop"),,3))
})


test_that("pop and hivpop+artpop are synchronised", {
  mod <- simmod(bw_fp)
  modR <- simmod(bw_fp, "R")
  expect_equal(colSums(mod[,,2,],,2),
               colSums(attr(mod, "hivpop"),,3) + colSums(attr(mod, "artpop"),,4))
  expect_equal(colSums(modR[,,2,],,2),
               colSums(attr(modR, "hivpop"),,3) + colSums(attr(modR, "artpop"),,4))
})
