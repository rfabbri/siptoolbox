function [x_proj_pca, x_proj_orig] = sip_pca_project(x_test, data_db, mean_db)
  x_proj_pca = data_db\(x_test-mean_db)       // pca space
  x_proj_orig = data_db*x_proj_pca + mean_db  // original space
endfunction
