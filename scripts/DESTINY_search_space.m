function DESTINY_search_space()
    tic; % Start timer
    solutions = generate_solutions(144); %desired capacity in MB
    toc; % End timer

    fprintf('Total number of solutions: %d\n', length(solutions));

    %example solution listing
    if ~isempty(solutions)
        disp('Example solution:');
        disp(solutions(6900));
    end

    % Filter solutions to get unique values of capacity, mat size and word width
    unique_capacity_in_mb = unique([solutions.capacity_in_mb]);
    unique_num_mats_col = unique([solutions.num_mats_col]);
    unique_word_width = unique([solutions.word_width]);

    fprintf('Possible values of capacity_in_mb: ');
    disp(sort(unique_capacity_in_mb));

    fprintf('Possible mat organisation n x n: ');
    disp(sort(unique_num_mats_col));

    fprintf('Possible values of word_width: ');
    disp(sort(unique_word_width));
end

function solutions = generate_solutions(size_in_mb)
    % Define the search space
    capacity_divisors = find(rem(size_in_mb, 1:size_in_mb) == 0); %capacity in MB or it's divisors
    word_width_options = 2.^(3:10);  % 8 to 1024 wordwidts
    mux_options = [1,2,4,8,64,128,256]; % MuxSenseAmp, MuxOutputLev1, MuxOutputLev2 values options
    mat_sizes = find(rem(144, (1:12).^2) == 0); %12x12 mats or its equal subcombinations, can be changed to any number!
    subbaray_set = [1,2,4,6,8,10,12]; %available subarrays per mat (nrow_subarray * ncol_subarray) (can be changed to any number)
    
    % Generate all combinations
    [C1,C2,C3,C4,C5,C6,C7] = ndgrid(capacity_divisors, word_width_options, mux_options, ...
        mux_options, mux_options, mat_sizes, subbaray_set);
    
    capacity_in_mb = C1(:);
    word_width = C2(:);
    MuxSenseAmp = C3(:);
    MuxOutputLev1 = C4(:);
    MuxOutputLev2 = C5(:);
    mat_size = C6(:);
    num_subarray = C7(:);
    
    % Fixed values
    associativity = 1; %1 way associative beacuse we focus on RAM
    stackdiecount = 1; %3D stack
    num_row_per_set = 1; %RAM 
    
    % Vectorized calculations
    capacity = capacity_in_mb * 2^20 * 8; %convert capacity to bits
    num_way_per_row = floor(associativity./num_row_per_set);
    num_mats = mat_size.^2;
    num_col_mat_active = mat_size;
    num_row_mat_active = mat_size;
    N_active_subarrays = num_subarray;
    
    Mux_levels = MuxSenseAmp .* MuxOutputLev1 .* MuxOutputLev2;
    num_address_bits = floor(log2(capacity./word_width/associativity/stackdiecount));
    num_address_bits_mat = num_address_bits - floor(log2(num_mats./(num_col_mat_active.*num_row_mat_active)));
    num_address_bits_gating = floor(log2(num_subarray./N_active_subarrays));
    num_address_bits_subarray = num_address_bits_mat - num_address_bits_gating;
    h = floor(log2(num_col_mat_active));
    v = floor(log2(num_row_mat_active));
    row = ((2.^num_address_bits_subarray * associativity) ./ Mux_levels .* num_way_per_row);
    col = ((word_width ./ 2.^(h+v)) ./ N_active_subarrays) .* Mux_levels .* num_way_per_row;
    calculated_size = num_mats .* num_subarray .* row .* col * stackdiecount;
    
    % Constrain: capacity = size and col and row are positive integers
    valid = abs(capacity - calculated_size) < 1e-6 & mod(row,1) == 0 & mod(col,1) == 0 & row > 0 & col > 0;
    
    % Create solution struct array
    solutions = struct('capacity_in_mb', num2cell(capacity_in_mb(valid)), ...
                       'word_width', num2cell(word_width(valid)), ...
                       'MuxSenseAmp', num2cell(MuxSenseAmp(valid)), ...
                       'MuxOutputLev1', num2cell(MuxOutputLev1(valid)), ...
                       'MuxOutputLev2', num2cell(MuxOutputLev2(valid)), ...
                       'num_mats_col', num2cell(mat_size(valid)), ...
                       'num_mats_row', num2cell(mat_size(valid)), ...
                       'size', num2cell(calculated_size(valid)), ...
                       'row', num2cell(row(valid)), ...
                       'col', num2cell(col(valid)), ...
                       'num_address_bits_subarray', num2cell(num_address_bits_subarray(valid)), ...
                       'num_subbarays', num2cell(num_subarray(valid)));
end