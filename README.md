<h1>
  Railway Degradation Analysis Overview
</h1>
I developed a simulation model to <b>analyze the degradation of railway segments</b> over a specified period. The model evaluates the wear level of each railway segment and determines which routes are worth preserving based on their average degradation after a given number of years.
I utilized <b>graph theory</b> to represent the railway network as an undirected graph, implementing adjacency lists for efficient traversal and analysis.
<h3>
  Method Description
</h3>
The goal of this analysis is to determine which segments of old railway routes should be preserved based on their wear levels over a specified period (K years). Each route is represented as a series of segments, each assigned a wear degree (D). A segment is considered in perfect condition with a wear degree of 0%, while it is deemed unusable when it reaches 100%.
<h3>
  Wear Progression Model
</h3>
The wear degree of each segment doubles annually. Thus, if a segment has an initial wear degree 𝐷, after K years, its wear degree will be 𝐷×2<sup>𝐾</sup>, with a maximum of 100%.
If a worn segment (degree > 0) is adjacent to a perfect segment (degree = 0), it will transfer half of its wear degree to the adjacent segment after one year.
If two adjacent segments are both worn, the higher wear degree will be used for the transfer. The wear degrees are updated iteratively over 
𝐾 years, applying the above rules for each segment and its neighbors.
<h3>
  Preservation Criteria
</h3>
After 𝐾 years, a route is deemed worthy of preservation if the average wear degree across all its segments is below a specified threshold 𝐿.
