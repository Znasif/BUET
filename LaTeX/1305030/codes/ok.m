function MotionBasedMultiObjectTrackingExample()
% Create System objects used for reading video, detecting moving objects,
% and displaying the results.
[obj,framePerSecond] = setupSystemObjects();

tracks = initializeTracks(); % Create an empty array of tracks.

objects = allObjects();

nextId = 1; % ID of the next track

objId = 1;

% Detect moving objects, and track them across video frames.
while ~isDone(obj.reader)
    frame = readFrame();
    [centroids, bboxes, mask] = detectObjects(frame);
    predictNewLocationsOfTracks();
    [assignments, unassignedTracks, unassignedDetections] = ...
        detectionToTrackAssignment();

    updateAssignedTracks();
    updateUnassignedTracks();
    deleteLostTracks();
    createNewTracks();

    displayTrackingResults();
    pause(.02);
end
showVelocity();

function [obj,framePerSecond] = setupSystemObjects()
        % Initialize Video I/O
        % Create objects for reading a video from a file, drawing the tracked
        % objects in each frame, and playing the video.

        % Create a video file reader.
        videoName = 'visiontraffic.avi';
        obj.reader = vision.VideoFileReader(videoName);
        
        info = VideoReader(videoName);
        
        dur = info.Duration;
        frameCount = info.NumberOfFrames;
        framePerSecond = info.FrameRate;
        p = sprintf('Frame number per second %.3f',framePerSecond);
        disp(p);

        % Create two video players, one to display the video,
        % and one to display the foreground mask.
        obj.maskPlayer = vision.VideoPlayer('Position', [100, 200, 500, 300]);
        obj.videoPlayer = vision.VideoPlayer('Position', [600, 200, 500, 300]);

        % Create System objects for foreground detection and blob analysis

        % The foreground detector is used to segment moving objects from
        % the background. It outputs a binary mask, where the pixel value
        % of 1 corresponds to the foreground and the value of 0 corresponds
        % to the background.

        obj.detector = vision.ForegroundDetector('NumGaussians', 3, ...
            'NumTrainingFrames', 40, 'MinimumBackgroundRatio', 0.7);

        % Connected groups of foreground pixels are likely to correspond to moving
        % objects.  The blob analysis System object is used to find such groups
        % (called 'blobs' or 'connected components'), and compute their
        % characteristics, such as area, centroid, and the bounding box.

        obj.blobAnalyser = vision.BlobAnalysis('BoundingBoxOutputPort', true, ...
            'AreaOutputPort', true, 'CentroidOutputPort', true, ...
            'MinimumBlobArea', 100);%area means number of pixels
 end

function tracks = initializeTracks()
        % create an empty array of tracks
        tracks = struct(...
            'id', {}, ...
            'bbox', {}, ...
            'kalmanFilter', {}, ...
            'age', {}, ...
            'totalVisibleCount', {}, ...
            'centroid', {} , ...
            'centroidP' , {} , ...
            'totalPixel' , {}, ...
            'consecutiveInvisibleCount', {});
end

function objects = allObjects()
        objects = struct(...
            'id', {}, ...
            'velocity' , {});
end    
            

function frame = readFrame()
        frame = obj.reader.step();
end

 function [centroids, bboxes, mask] = detectObjects(frame)

        % Detect foreground.
        mask = obj.detector.step(frame);

        % Apply morphological operations to remove noise and fill in holes.
        mask = imopen(mask, strel('rectangle', [3,3]));
        mask = imclose(mask, strel('rectangle', [15, 15]));
        mask = imfill(mask, 'holes');

        % Perform blob analysis to find connected components.
        [~, centroids, bboxes] = obj.blobAnalyser.step(mask);
 end

function predictNewLocationsOfTracks()
        for i = 1:length(tracks)
            bbox = tracks(i).bbox;

            % Predict the current location of the track.
            predictedCentroid = predict(tracks(i).kalmanFilter);

            % Shift the bounding box so that its center is at
            % the predicted location.
            predictedCentroid = int32(predictedCentroid) - bbox(3:4) / 2;
            tracks(i).bbox = [predictedCentroid, bbox(3:4)];
        end
end

function [assignments, unassignedTracks, unassignedDetections] = ...
            detectionToTrackAssignment()

        nTracks = length(tracks);
        nDetections = size(centroids, 1);

        % Compute the cost of assigning each detection to each track.
        cost = zeros(nTracks, nDetections);
        for i = 1:nTracks
            cost(i, :) = distance(tracks(i).kalmanFilter, centroids);
        end

        % Solve the assignment problem.
        costOfNonAssignment = 20;
        [assignments, unassignedTracks, unassignedDetections] = ...
            assignDetectionsToTracks(cost, costOfNonAssignment);
end

function updateAssignedTracks()
        numAssignedTracks = size(assignments, 1);
        for i = 1:numAssignedTracks
            trackIdx = assignments(i, 1);
            detectionIdx = assignments(i, 2);
            centroid = centroids(detectionIdx, :);
            bbox = bboxes(detectionIdx, :);
            tracks(trackIdx).centroidP = tracks(trackIdx).centroid;   %new Line
            % Correct the estimate of the object's location
            % using the new detection.
            correct(tracks(trackIdx).kalmanFilter, centroid);

            % Replace predicted bounding box with detected
            % bounding box.
            tracks(trackIdx).bbox = bbox;
            
            tracks(trackIdx).centroid = centroid; % new line

            % Update track's age.
            tracks(trackIdx).age = tracks(trackIdx).age + 1;

            % Update visibility.
            tracks(trackIdx).totalVisibleCount = ...
                tracks(trackIdx).totalVisibleCount + 1;
            tracks(trackIdx).consecutiveInvisibleCount = 0;
        end
end

function updateUnassignedTracks()
        for i = 1:length(unassignedTracks)
            ind = unassignedTracks(i);
            tracks(ind).age = tracks(ind).age + 1;
            tracks(ind).consecutiveInvisibleCount = ...
                tracks(ind).consecutiveInvisibleCount + 1;
        end
end

 function deleteLostTracks()
        if isempty(tracks)
            return;
        end

        invisibleForTooLong = 20;
        ageThreshold = 8;

        % Compute the fraction of the track's age for which it was visible.
        ages = [tracks(:).age];
        totalVisibleCounts = [tracks(:).totalVisibleCount];
        visibility = totalVisibleCounts ./ ages;

        % Find the indices of 'lost' tracks.
        lostInds = (ages < ageThreshold & visibility < 0.6) | ...
            [tracks(:).consecutiveInvisibleCount] >= invisibleForTooLong;
      s1 = length(lostInds); 
      %disp(lostInds);
        
    for tia = 1:s1
        t = lostInds(tia);
      if(t ~= 0)
        velo = tracks(t).totalPixel/(tracks(t).age-1);
        tempo = sprintf('Average velocity of object no %d is %.3f',tracks(t).id,velo);
        newObject = struct(...
                'id', tracks(t).id, ...
                'velocity', velo * framePerSecond);
        objects([objects.id] == tracks(t).id) = [];
        objId = length(objects) + 1;
        objects(objId) = newObject;
        objId = objId + 1;
        %disp(tempo);
      end
    end

        % Delete lost tracks.
        %disp(lostInds);
        tracks = tracks(~lostInds);
 end

 function createNewTracks()
        centroids = centroids(unassignedDetections, :);
        bboxes = bboxes(unassignedDetections, :);

        for i = 1:size(centroids, 1)

            centroid = centroids(i,:);
            bbox = bboxes(i, :);

            % Create a Kalman filter object.
            kalmanFilter = configureKalmanFilter('ConstantVelocity', ...
                centroid, [200, 50], [100, 25], 100);

            % Create a new track.
            newTrack = struct(...
                'id', nextId, ...
                'bbox', bbox, ...
                'kalmanFilter', kalmanFilter, ...
                'age', 1, ...
                'totalVisibleCount', 1, ...
                'centroid' , centroid , ...%new Line
                'centroidP' , centroid , ...
                'totalPixel' , 0 , ...
                'consecutiveInvisibleCount', 0);

            % Add it to the array of tracks.
            tracks(end + 1) = newTrack;

            % Increment the next id.
            nextId = nextId + 1;
           % e = sprintf('created a new track of id %d',nextId-1);
           % disp(e);
        end
 end

function displayTrackingResults()
        % Convert the frame and the mask to uint8 RGB.
        frame = im2uint8(frame);
        mask = uint8(repmat(mask, [1, 1, 3])) .* 255;

        minVisibleCount = 8;
        if ~isempty(tracks)

            % Noisy detections tend to result in short-lived tracks.
            % Only display tracks that have been visible for more than
            % a minimum number of frames.
            reliableTrackInds = ...
                [tracks(:).totalVisibleCount] > minVisibleCount;
            reliableTracks = tracks(reliableTrackInds);

            % Display the objects. If an object has not been detected
            % in this frame, display its predicted bounding box.
            if ~isempty(reliableTracks)
                % Get bounding boxes.
                bboxes = cat(1, reliableTracks.bbox);

                % Get ids.
                ids = int32([reliableTracks(:).id]);

                % Create labels for objects indicating the ones for
                % which we display the predicted rather than the actual
                % location.
                labels = cellstr(int2str(ids'));
                predictedTrackInds = ...
                    [reliableTracks(:).consecutiveInvisibleCount] > 0;
                isPredicted = cell(size(labels));
                isPredicted(predictedTrackInds) = {' predicted'};
                labels = strcat(labels, isPredicted);
                
                
                originalTrackInds = ...
                    [reliableTracks(:).consecutiveInvisibleCount] <= 0;
                originalTracks = tracks(originalTrackInds);
               len = length(originalTracks);
               isOriginal = zeros(1,len);
             
               for j = 1:len
                    x = (originalTracks(j).centroid(1) - originalTracks(j).centroidP(1));
                    y = (originalTracks(j).centroid(2) - originalTracks(j).centroidP(2));
                    z = (sqrt(x^2 + y^2)) * framePerSecond;
                    isOriginal(j) = (z);
                    z = num2str(z);
                    labels(j) = strcat(labels(j),' Velocity : ');
                    labels(j) = strcat(labels(j),z);
                    labels(j) = strcat(labels(j),' p/s');
               end
            
               
                
                % Draw the objects on the frame.
                frame = insertObjectAnnotation(frame, 'rectangle', ...
                    bboxes, labels);
                siz = length(tracks);
               
                for t = 1:siz
                   diff = (tracks(t).centroid - ...
                        tracks(t).centroidP);
                    diff1 = sqrt(diff(1)^2 + diff(2)^2);
                    tracks(t).totalPixel = tracks(t).totalPixel + diff1;
                    p = sprintf('Object no %d pixel per frame %.3f age %d',tracks(t).id,diff1,tracks(t).age);
                   % disp(p);
                end
               
                % Draw the objects on the mask.
                mask = insertObjectAnnotation(mask, 'rectangle', ...
                    bboxes, labels);
            end
        end

        % Display the mask and the frame.
        obj.maskPlayer.step(mask);
        obj.videoPlayer.step(frame);
       % distCovered(k) = sqrt(sum((centroids(k,:)-centroids_old(k,:)).^2));
end

function showVelocity()
    s = length(tracks);
    for ti = 1:s
        velo = tracks(ti).totalPixel/(tracks(ti).age-1);
       newObject = struct(...
                'id', tracks(ti).id, ...
                'velocity', velo * framePerSecond);
        objects([objects.id] == tracks(ti).id) = [];
        objId = length(objects) + 1;
        objects(objId) = newObject;
        objId = objId + 1;
    end
    s = length(objects);
    for ti = 1:s
        temp = sprintf('Average velocity of object no %d is %.3f pixel per second', ...
            objects(ti).id,objects(ti).velocity);
        disp(temp);
    end
end
end

