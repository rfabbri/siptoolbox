data = [
     0     1
     0     1];
// manifold is line from (0,0) to (1,1)

[data_db, mean_db]= sip_pca(data,1)

x_test = [0; 0];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [0,0]'

x_test = [1; 1];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [1,1]'

x_test = [0.5; 0.5];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [0.5,0.5]'

x_test = [0.4; 0.6];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [0.5,0.5]'

// ---------------------------
// increasing the dimension to 2 should make the projection operator the
// identity, as the manifold is now the entire plane.

[data_db, mean_db]= sip_pca(data,2)

x_test = [0; 0];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [0,0]'

x_test = [1; 1];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [1,1]'

x_test = [0.5; 0.5];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be [0.5,0.5]'

x_test = [0.4; 0.6];
[x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
disp 'result of projection in original space should be *** [0.4,0.6] ***' 
